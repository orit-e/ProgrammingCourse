/******************************************************************************
 											
				File name:			watchdog.c			
				Written by:     	Orit			
				Reviewed by:    	
 											
 ******************************************************************************/
#define _POSIX_C_SOURCE 200112L

#include <signal.h>     /*  kill(), sigaction(), struct sigaction, 
                            sigemptyset(), sigaddset(), sigtimedwait(), 
                            sigset_t                                        */
#include <sys/types.h>  /*  kill(), fork(), wait(), getpid(), getppid()     */
#include <unistd.h>     /*  fork(), execvp(), sleep(), getpid(), getppid()  */
#include <stdio.h>      /*  fprintf(), sprintf()                            */
#include <sys/wait.h>   /*  wait()                                          */
#include <stdlib.h>     /*  abort(), setenv(), getenv(), unsetenv()         */
#include <pthread.h>    /*  pthread_create(), pthread_join(),
                            pthread_mutex_init(), pthread_mutex_destroy(),
                            pthread_mutex_lock(), pthread_mutex_unlock(),
                            pthread_cond_init(), pthread_cond_destroy(),
                            pthread_cond_brodcast(), pthread_cond_wait()    */
#include <fcntl.h>      /*  For O_* constants                               */
#include <semaphore.h>  /*  sem_open(), sem_wait(), sem_post(), 
                            sem_unlink(), sem_timedwait()                   */
#include <time.h>       /*  time(), sem_timedwait(), struct timespec        */
#include <stdatomic.h>  /*  atomic_fetch_add(), atomic_store()              */
#include <errno.h>      /*  errno                                           */
#include <assert.h>     /*  assert()                                        */

#include "scheduler.h"
#include "uid.h"
#include "watchdog.h"

/******************************************************************************/
/*					defines, enum and typedefs declarations:				  */
#define NUM_OF_ITER (5)
#define CLEANUP_ARR_SIZE (5)

enum IS_WD
{
    IS_WD_FALSE,
    IS_WD_TRUE
};

enum WD_TASK_IS_SECCESS
{
    WD_TASK_SUCCESS,
    WD_TASK_FAIL
};

enum WD_SEM_TIMED_WAIT_IS_SUCCESS
{
    WD_SEM_TIMED_WAIT_SUCCESS,
    WD_SEM_TIMED_WAIT_FAIL
};

enum WHICH_SEMAPHORE
{
    SEM_WD_START,
    SEM_WD_STOP
};

enum WD_IS_FIRST_PROC
{
    WD_FIRST_PROC,
    WD_NOT_FIRST_PROC
};

enum WD_CLEAN_UP_NUMBERS
{
    WD_STOP_USER_PROC_CLEAN_UP = 0,
    WD_START_AFTER_ENV_CREATED_FOR_FIRST_PROC = 1,
    WD_STOP_WD_PROC_CLEAN_UP = 3,
    WD_START_AFTER_SCHEDULER_CREATED = 3,
    WD_START_AFTER_SIG_HANDLERS_SET = 4
};

typedef int (*wd_generic_func_1_t)(void);
typedef void (*wd_generic_func_2_t)(void);
typedef void (*sig_func_t)(int, siginfo_t *, void *);
typedef char *(*get_target_t)(void);
typedef int (*task_func_t)(void *);


/******************************************************************************/
/*		        		Static functions declarations:		        		  */
/*Spawn function:		        		                                      */
static int SpawnChild(void);

/*Signal Handler functions:		        		                              */
static void InitSigHandlers(void);
static void InitSigHandlerSIGUSR2ForWDProc(void);
static void SigHandlerSIGUSR1(int signum, siginfo_t *info, void *ucontext);
static void SigHandlerSIGUSR2WDProc(int signum, siginfo_t *info, 
                                    void *ucontext);
static void ChangeSIGUSR1SigHandlerBackEnv(void);

/*WDStart get sem addr and init routine functions:		        	          */
static int WDStartFirstCallInit(void);
static int WDStartNotFirstCallInit(void);
static int GetSemAddr(int flags);
static void SemUnlinkForUserProcessEnv(void);

/*WD Scheduler functions:		        		                              */
static int WDIinitSchedular(void);
static void DestroySchedulerEnv(void);
static int OpenThreadAndExecutScheduler(void);
static int SchedulerExecuting(void);
static void *ExecuteSchedular(void *ignored);
static int InsertTaskToScheduler(task_func_t task_func, size_t interval);
static int TaskSendSignal(void *ignored);
static int TaskCheckCounter(void *ignored);
static int RestartPartner(void);

/*WD Env functions:		        		                                      */
static int SetEnvVars(void);
static void SetSemNameAsEnv(int which_sem);
static void SetWDExecNameAsEnv(void);
static void SetUserExecNameAsEnv(void);
static char *GetWDExecNameFromEnvVar(void);
static char *GetUserExecNameFromEnvVar(void);
static char *GetSemUniqueNameFromEnvVarStart(void);
static char *GetSemUniqueNameFromEnvVarStop(void);

/*WDStop Sync functions:		        		                              */
static int SynchronizeStopUserProc(void);
static int SynchronizeStopWDProc(void);
static void JoinSchedulerThread(void);

/*WDStop cleanup routine functions:		        		                      */
static int CleanUpAndDestroyUserProc(void);
static int CleanUpAndDestroyWDProc(void);
static void CleanUpAndDestroy(int cleanup_num);

/*WD interval wait function:		        		                          */
static int SemTimedWaitInterval(int which_sem);

/*Do nothing functions for function arrays                                    */
static void DoNothingReturnVoid(void);
static int DoNothingReturnInt(void);

/******************************************************************************/
/*  		        Global Variables And Function Arrays:		   	          */
extern char **environ;
char **wd_argv;

char *sem_wd_uid_name[2] = 
{
    "SEM_WD_UID_NAME_START",
    "SEM_WD_UID_NAME_STOP"
};
char *wd_prog_exec = "WD_PROG_EXEC";
char *wd_user_prog_exec = "WD_USER_PROG_EXEC";

struct sigaction *default_sigusr1_action;
struct sigaction *default_sigusr2_action;

scheduler_t *wd_scheduler;
pthread_t wd_scheduler_thread_id;
atomic_size_t counter;
sem_t *sem_wd[2];
pid_t partner_proc_pid;

int is_wd_proc = IS_WD_FALSE;
int is_first_proc = WD_NOT_FIRST_PROC;


wd_generic_func_2_t init_2nd_sighandler_for_wd_proc[2] = 
{
    DoNothingReturnVoid,
    InitSigHandlerSIGUSR2ForWDProc
};
wd_generic_func_1_t wd_init_routine[2] = 
{
    WDStartNotFirstCallInit,
    WDStartFirstCallInit
};
get_target_t get_target_func[2] = 
{
    GetWDExecNameFromEnvVar, 
    GetUserExecNameFromEnvVar
};
wd_generic_func_1_t execute_scheduler[2] = 
{
    OpenThreadAndExecutScheduler,
    SchedulerExecuting
};
wd_generic_func_1_t sync_proc_for_wd[2] = 
{
    DoNothingReturnInt, 
    SynchronizeStopWDProc
};
wd_generic_func_1_t sync_proc_for_user[2] = 
{
    SynchronizeStopUserProc,
    DoNothingReturnInt
};
wd_generic_func_1_t cleanup_func[2] = 
{
    CleanUpAndDestroyUserProc,
    CleanUpAndDestroyWDProc
};
wd_generic_func_2_t cleanup_routine[5] = 
{
    JoinSchedulerThread,
    SemUnlinkForUserProcessEnv,
    UnsetTargets,
    DestroySchedulerEnv,
    ChangeSIGUSR1SigHandlerBackEnv
};


/******************************************************************************/
/*								WD functions:						          */
int WDStart(char *argv[])
{
    int status = WD_START_SUCCESS;

    assert(NULL != argv);

    InitSigHandlers();

    wd_argv = argv;

    status = WDIinitSchedular();
    if(WD_START_SUCCESS != status)
    {
        CleanUpAndDestroy(WD_START_AFTER_SIG_HANDLERS_SET);

        return WD_START_ALLOC_FAIL;
    }

    is_first_proc = (NULL == getenv(sem_wd_uid_name[SEM_WD_START]));
    status = wd_init_routine[is_first_proc]();
    if(WD_START_SUCCESS != status)
    {
        return status;
    }

    status = execute_scheduler[is_wd_proc]();

    return status;
}

int WDStop()
{
    int status = WD_STOP_SUCCESS;

    status = sync_proc_for_user[is_wd_proc]();
    if(WD_STOP_SUCCESS != status)
    {
        return status;
    }

    status = cleanup_func[is_wd_proc]();
    if(WD_STOP_SUCCESS != status)
    {
        return status;
    }

    status = sync_proc_for_wd[is_wd_proc]();

    return status;
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
/* Spawn function:		        		                                      */
static int SpawnChild(void)
{
    pid_t child_pid = 0;
    int status = 0;

    child_pid = fork();
    if(-1 == child_pid)
    {
        return WD_START_FORK_FAIL;
    }
    if(0 == child_pid)
    {
        wd_argv[0] = get_target_func[is_wd_proc]();
        execv(wd_argv[0], wd_argv);
        fprintf (stderr, "An error occurred in execv\n");
        abort();
    }
    
    partner_proc_pid = child_pid;
    
    status = SemTimedWaitInterval(SEM_WD_START);
    if(WD_SEM_TIMED_WAIT_SUCCESS != status)
    {
        return WD_START_CHILD_PROCESS_NOT_RESPOND;
    }

    return status;
}

/* Signal Handler functions:		        	                              */
static void InitSigHandlers(void)
{
    struct sigaction sigusr1_action = {0};

    sigusr1_action.sa_flags = SA_SIGINFO;
    sigusr1_action.sa_sigaction = SigHandlerSIGUSR1;
    sigaction(SIGUSR1, &sigusr1_action, default_sigusr1_action);

    init_2nd_sighandler_for_wd_proc[is_wd_proc]();
}

static void InitSigHandlerSIGUSR2ForWDProc(void)
{
    struct sigaction sigusr2_action = {0};

    sigusr2_action.sa_flags = SA_SIGINFO;
    sigusr2_action.sa_sigaction = SigHandlerSIGUSR2WDProc;
    sigaction(SIGUSR2, &sigusr2_action, default_sigusr1_action);
}

static void SigHandlerSIGUSR1(int signum, siginfo_t *info, void *ucontext)
{
    if(partner_proc_pid == info->si_pid)
    {
        atomic_fetch_add(&counter, 1);
    }
    (void)signum;
    (void)ucontext;
}

static void SigHandlerSIGUSR2WDProc(int signum, siginfo_t *info, void *ucontext)
{
    if(partner_proc_pid == info->si_pid)
    {   
        SchedulerStop(wd_scheduler);
    }
    (void)signum;
    (void)ucontext;
}

static void ChangeSIGUSR1SigHandlerBackEnv()
{
    sigaction(SIGUSR1, default_sigusr1_action, NULL);
}

/* WDStart get sem addr and init routine functions:		        	          */
static int WDStartFirstCallInit(void)
{
    int status = WD_START_SUCCESS;
    int sem_open_flags = 0;

    SetEnvVars();

    sem_open_flags = O_CREAT | O_EXCL;
    status = GetSemAddr(sem_open_flags);
    if(WD_START_SUCCESS != status)
    {
        return status;
    }

    status = SpawnChild();
    if(WD_START_SUCCESS != status)
    {
        int cleanup_number[2] = {WD_START_AFTER_SCHEDULER_CREATED, 
                                    WD_START_AFTER_ENV_CREATED_FOR_FIRST_PROC};

        CleanUpAndDestroy(cleanup_number[is_first_proc]);

        return WD_START_ALLOC_FAIL;
    }

    return status;  
}

static int WDStartNotFirstCallInit(void)
{
    int status = WD_START_SUCCESS;
    int sem_open_flags = 0;

    sem_open_flags = O_CREAT;
    status = GetSemAddr(sem_open_flags);
    if(WD_START_SUCCESS != status)
    {
        return status;
    }

    partner_proc_pid = getppid();
    sem_post(sem_wd[SEM_WD_START]);  

    return status; 
}

static int GetSemAddr(int flags)
{
    char *sem_wd_name_start = GetSemUniqueNameFromEnvVarStart();
    char *sem_wd_name_stop = GetSemUniqueNameFromEnvVarStop();

    sem_wd[SEM_WD_START] = sem_open(sem_wd_name_start, flags, 0666, 0);
    if(SEM_FAILED == sem_wd[SEM_WD_START])
    {
        int cleanup_number[2] = {WD_START_AFTER_SCHEDULER_CREATED, 
                                    WD_START_AFTER_ENV_CREATED_FOR_FIRST_PROC};

        CleanUpAndDestroy(cleanup_number[is_first_proc]);

        return WD_START_SEM_FAIL;
    }

    sem_wd[SEM_WD_STOP] = sem_open(sem_wd_name_stop, flags, 0666, 0);
    if(SEM_FAILED == sem_wd[SEM_WD_STOP])
    {
        int cleanup_number[2] = {WD_START_AFTER_SCHEDULER_CREATED, 
                                    WD_START_AFTER_ENV_CREATED_FOR_FIRST_PROC};

        CleanUpAndDestroy(cleanup_number[is_first_proc]);

        return WD_START_SEM_FAIL;
    }

    return WD_START_SUCCESS;
}

static void SemUnlinkForUserProcessEnv()
{
    sem_unlink(GetSemUniqueNameFromEnvVarStart());
    sem_unlink(GetSemUniqueNameFromEnvVarStop());
}

/* WD Scheduler functions:		        		                              */
static int WDIinitSchedular(void)
{
    int status = WD_START_SUCCESS;

    wd_scheduler = SchedulerCreate();
    if(NULL == wd_scheduler)
    {
        return WD_START_ALLOC_FAIL;
    }

    status = InsertTaskToScheduler(TaskSendSignal, 1);
    if(WD_START_SUCCESS != status)
    {
        return WD_START_ALLOC_FAIL;
    }

    status = InsertTaskToScheduler(TaskCheckCounter, 10);
    if(WD_START_SUCCESS != status)
    {
        return WD_START_ALLOC_FAIL;
    }

    return status;
}

static void DestroySchedulerEnv()
{
    SchedulerDestroy(wd_scheduler);
}

static int OpenThreadAndExecutScheduler(void)
{
    int status = 0;

    status = pthread_create(&wd_scheduler_thread_id, NULL,
                            ExecuteSchedular, NULL);
    if(WD_START_SUCCESS != status)
    {
        int cleanup_number[2] = {WD_START_AFTER_SCHEDULER_CREATED, 
                                    WD_START_AFTER_ENV_CREATED_FOR_FIRST_PROC};

        CleanUpAndDestroy(cleanup_number[is_first_proc]);

        return WD_START_PTHREAD_CREATE_FAIL;
    }

    return status;
}

static int SchedulerExecuting(void)
{
    int status = WD_START_SUCCESS;

    status = SchedulerExecute(wd_scheduler);
    
    sem_post(sem_wd[SEM_WD_STOP]);

    return status;
}

static void *ExecuteSchedular(void *ignored)
{
    int status = SchedulerExecuting();

    (void)ignored;

    return (void *)((size_t)status);
}

static int InsertTaskToScheduler(task_func_t task_func, size_t interval)
{
    ilrd_uid_t task_uid = {0};
    ilrd_uid_t bad_uid = BadUIDCreate();

    task_uid = SchedulerInsertTask(wd_scheduler, task_func, 
                                    NULL, interval);
    if(1 == UIDISEqual(&task_uid, &bad_uid))
    {
        return WD_START_ALLOC_FAIL;
    }

    return WD_START_SUCCESS;
}

static int TaskSendSignal(void *ignored)
{
    kill(partner_proc_pid, SIGUSR1);

    (void)ignored;

    return WD_TASK_SUCCESS;
}

static int TaskCheckCounter(void *ignored)
{
    int status = WD_TASK_SUCCESS;

    if(0 < counter)
    {
        atomic_store(&counter, 0);
    }
    else
    {
        status = RestartPartner();
    }

    (void)ignored;

    return status;
}

static int RestartPartner(void)
{
    int status = WD_TASK_SUCCESS;

    status = kill(partner_proc_pid, SIGTERM);
    if(0 == status)
    {
        sleep(1);
        status = kill(partner_proc_pid, SIGKILL);
    }
    if(-1 == status && ESRCH != errno)
    {
        return WD_TASK_SUCCESS;
    }
    if(getppid() != partner_proc_pid)
    {
        do
        {
            status = waitpid(partner_proc_pid, NULL, 0);
        } while (0 != status && EINTR == errno);
    }

    SpawnChild();

    return WD_TASK_SUCCESS;
}

/* WD Env functions:		        	                                      */
static int SetEnvVars()
{   
    SetSemNameAsEnv(SEM_WD_START);
    SetSemNameAsEnv(SEM_WD_STOP);
    SetWDExecNameAsEnv();
    SetUserExecNameAsEnv();
    
    return WD_START_SUCCESS;
}

static void SetSemNameAsEnv(int which_sem)
{
    ilrd_uid_t wd_uid = UIDCreate();
    char sem_wd_buffer[256] = {0};

    sprintf(sem_wd_buffer, "_%d%lu%ld%lu", wd_uid.pid, wd_uid.thread_id, 
                                            wd_uid.time_stamp, wd_uid.counter);
    setenv(sem_wd_uid_name[which_sem], sem_wd_buffer, 0);
}

static void SetWDExecNameAsEnv()
{
    char *wd_proc_path = 
                "/home/user/Documents/git/projects/watchdog/watchdog.out";

    setenv(wd_prog_exec, wd_proc_path, 0);   
}

static void SetUserExecNameAsEnv()
{
    har user_proc_path[256] = {0};

    sprintf(user_proc_path, "%s%s", getenv("PWD"), (wd_argv[0] + 1));
    setenv(wd_user_prog_exec, user_proc_path, 0);
}

static void UnsetTargets(void)
{
    unsetenv(sem_wd_uid_name[SEM_WD_START]);
    unsetenv(sem_wd_uid_name[SEM_WD_STOP]);
    unsetenv(wd_prog_exec);
    unsetenv(wd_user_prog_exec);
}

static char *GetWDExecNameFromEnvVar(void)
{
    return getenv(wd_prog_exec);
}

static char *GetUserExecNameFromEnvVar(void)
{
    return getenv(wd_user_prog_exec);
}

static char *GetSemUniqueNameFromEnvVarStart(void)
{
    return getenv(sem_wd_uid_name[SEM_WD_START]);
}

static char *GetSemUniqueNameFromEnvVarStop(void)
{
    return getenv(sem_wd_uid_name[SEM_WD_STOP]);
}

/* WDStop Sync functions:		        		                              */
static int SynchronizeStopUserProc(void)
{
    int status = WD_STOP_SUCCESS;
    int num_of_iterations = 0;
    
    do
    {
        kill(partner_proc_pid, SIGUSR2);
    }
    while(NUM_OF_ITER > num_of_iterations++);
    status = SemTimedWaitInterval(SEM_WD_STOP);
    if(WD_STOP_SUCCESS != status)
    {
        return WD_STOP_FAIL;
    }
    
    SchedulerStop(wd_scheduler);
    status = SemTimedWaitInterval(SEM_WD_STOP);
    if(WD_STOP_SUCCESS != status)
    {
        return WD_STOP_FAIL;
    }
    
    SemTimedWaitInterval(SEM_WD_STOP);

    return status;
}

static int SynchronizeStopWDProc(void)
{
    sem_post(sem_wd[SEM_WD_STOP]);

    return WD_STOP_SUCCESS;
}

static void JoinSchedulerThread(void)
{
    pthread_join(wd_scheduler_thread_id, NULL);
}

/* WDStop cleanup routine functions:		        	                      */
static int CleanUpAndDestroyUserProc(void)
{
    int status = CleanUpAndDestroy(WD_STOP_USER_PROC_CLEAN_UP);

    return status;
}

static int CleanUpAndDestroyWDProc(void)
{
    CleanUpAndDestroy(WD_STOP_WD_PROC_CLEAN_UP);

    return WD_STOP_SUCCESS;
}

static void CleanUpAndDestroy(int cleanup_num)
{
    for(; CLEANUP_ARR_SIZE > cleanup_num; ++cleanup_num)
    {
        cleanup_routine[cleanup_num]();
    }
}

/* WDStop interval wait functions:		        		                      */
static int SemTimedWaitInterval(int which_sem)
{
    struct timespec timeout = {0};
    time_t start_time = time(NULL);
    int status = WD_SEM_TIMED_WAIT_SUCCESS;

    timeout.tv_sec = time(NULL) + WD_TIMED_WAIT;
    do
    {
        status = sem_timedwait(sem_wd[which_sem], &timeout);
    }
    while(WD_SEM_TIMED_WAIT_SUCCESS != status && 
            WD_TIMED_WAIT > (time(NULL) - start_time));

    return status;
}

/*Do nothing functions for function arrays                                    */
static void DoNothingReturnVoid(void)
{
    return;
}

static int DoNothingReturnInt(void)
{
    return 0;
}