#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "watchdog.h"
#include "tests.h"
#include "scheduler.h"

int TestHandlers(char *argv[]);
int TestSchedulerAndTasks(char *argv[]);
int TestWDStartStop(char *argv[]);
int TestIntegraton4();
int TestIntegraton5();
int TestIntegraton6();

typedef struct wd_metadata
{
    struct sigaction *default_sigusr1_action;
    struct sigaction *default_sigusr2_action;
    scheduler_t *scheduler;
    pthread_t wd_scheduler_thread_id;
    sem_t *sem_addr;
    pid_t partner_pid;
    char **argv;
}wd_metadata_t;


extern int counter;
extern int exit_status;
extern wd_metadata_t wd_metadata;
extern char **environ;

/*
int DataCMP(const void *data1, const void *data2, void *param);
int IsMatch(void *data, void *param);

typedef struct data_struct
{
	int num;
	char letter;
}data_struct_t;

heap_ty *heap = NULL;
data_struct_t a = {1, 'a'};
data_struct_t b = {2, 'b'};
data_struct_t c = {3, 'c'};
data_struct_t d = {4, 'd'};
data_struct_t d1 = {4, 'd'};
data_struct_t e = {5, 'e'};
data_struct_t f = {6, 'f'};
data_struct_t f1 = {6, 'f'};
data_struct_t g = {7, 'g'};
data_struct_t bigger = {10, 'i'};
data_struct_t smaller = {0, 'I'};
int param = 0;
char match_param = 0;

int DataCMP(const void *data1, const void *data2, void *param)
{
	data_struct_t *struct1 = (data_struct_t *)data1;
	data_struct_t *struct2 = (data_struct_t *)data2;
	
	return (struct1->num - struct2->num + *(int *)param);
}

int IsMatch(void *data, void *match_param)
{
	data_struct_t *my_struct = (data_struct_t *)data;

	return (my_struct->letter == *(char *)match_param);
}
*/
int main(int argc, char *argv[])
{
/*	RunTest(TestHandlers(argv),			"TestHandlers");
	RunTest(TestSchedulerAndTasks(argv),			"TestSchedulerAndTasks");*/
	RunTest(TestWDStartStop(argv),			"TestWDStartStop");
/*	RunTest(TestIntegraton4(),			"TestIntegraton4");
	RunTest(TestIntegraton5(),			"TestIntegraton5");
	RunTest(TestIntegraton6(),			"TestIntegraton6");*/

	(void)argc;
	(void)argv;

    return 0;
}
/*
int TestHandlers(char *argv[])
{    
    int result = 0;
    union sigval my_sig;
	
    my_sig.sival_int = getpid();
	result += (WD_START_SUCCESS != WDStart(argv));

    result += (0 != counter);
    sigqueue(getpid(), SIGUSR1, my_sig);
    result += (1 != counter);
	
    sigqueue(getpid(), SIGUSR2, my_sig);
    result += (WD_STOP_SUCCESS != exit_status);

	
	return result;
}

int TestSchedulerAndTasks(char *argv[])
{
    wd_metadata_t *wd_metadata = NULL;
	char *argv_stab[] = {"./stab.out", NULL};
	static union sigval my_sig;
	pid_t stab_porc_pid = 0;
	int wstatus = 0;
	int result = 0;
	
	my_sig.sival_int = getpid();

	result += (WD_START_SUCCESS != WDStart(argv));

	stab_porc_pid = fork();
	if(0 == stab_porc_pid)
	{
		execvp("./stab.out", argv_stab);
		printf("Not good!\n");
	}

	wd_metadata = (wd_metadata_t *)pthread_getspecific(wd_key);
	wd_metadata->partner_pid = stab_porc_pid;
	pthread_setspecific(wd_key, (void *)wd_metadata);

	
	result += (0 != SchedulerExecute(wd_metadata->scheduler));

	sigqueue(wd_metadata->partner_pid, SIGUSR2, my_sig);
	SchedulerDestroy(wd_metadata->scheduler);
	free(wd_metadata);

	result += (0 != counter);
    result += (WD_STOP_SUCCESS != exit_status);

	wait(&wstatus);
	result += (1 != WIFEXITED(wstatus));
	
	return result;
}
*/
int TestWDStartStop(char *argv[])
{
	int result = 0;
	int sleep_time = 60;
	
	
	WDStart(argv);

	while(0 != sleep_time)
	{
		sleep_time = sleep(sleep_time);
	}

	result = (WD_STOP_SUCCESS != WDStop());

	return result;
}
/*
int TestIntegraton4()
{
	int result = 0;
	
	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &f));
	result += (0 != HeapPush(heap, &c));
	result += (0 != HeapPush(heap, &d));
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapIsEmpty((const heap_ty *)heap));
	result += (4 != HeapSize((const heap_ty *)heap));
	HeapPop(heap);
	result += (&d != HeapPeek((const heap_ty *)heap));
	result += (3 != HeapSize((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton5()
{
	int result = 0;

	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &f));
	result += (0 != HeapPush(heap, &c));
	result += (0 != HeapPush(heap, &d));
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapPush(heap, &g));
	result += (0 != HeapIsEmpty((const heap_ty *)heap));
	result += (5 != HeapSize((const heap_ty *)heap));
	result += (&g != HeapPeek((const heap_ty *)heap));
	HeapPop(heap);
	result += (&f != HeapPeek((const heap_ty *)heap));
	result += (4 != HeapSize((const heap_ty *)heap));
	HeapPop(heap);
	result += (&d != HeapPeek((const heap_ty *)heap));
	result += (3 != HeapSize((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton6()
{
	int result = 0;

	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &g));
	result += (0 != HeapPush(heap, &d));
	result += (0 != HeapPush(heap, &f));
	result += (0 != HeapPush(heap, &c));
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapPush(heap, &b));
	result += (0 != HeapPush(heap, &e));
	result += (7 != HeapSize((const heap_ty *)heap));
	result += (&g != HeapPeek((const heap_ty *)heap));
	match_param = 'a';
	result += (&a != HeapRemove(heap, &IsMatch, (void *)&match_param));
	result += (&g != HeapPeek((const heap_ty *)heap));
	result += (6 != HeapSize((const heap_ty *)heap));
	match_param = 'g';
	result += (&g != HeapRemove(heap, &IsMatch, (void *)&match_param));
	match_param = 'h';
	result += (NULL != HeapRemove(heap, &IsMatch, (void *)&match_param));
	result += (&f != HeapPeek((const heap_ty *)heap));
	result += (5 != HeapSize((const heap_ty *)heap));
	HeapPop(heap);
	result += (&e != HeapPeek((const heap_ty *)heap));

	HeapDestroy(heap);
	
	return result;
}*/