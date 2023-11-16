#define _POSIX_C_SOURCE 199309L

#include <signal.h>     /*  kill(), sigaction(), struct sigaction   */
#include <sys/types.h>  /*  kill(), fork(), wait()                  */
#include <unistd.h>     /*  fork(), execvp(), sleep(), pause()      */
#include <stdio.h>      /*  printf(), fprintf()                     */
#include <sys/wait.h>   /*  wait()                                  */
#include <stdlib.h>     /*  abort()                                 */

extern char **environ;

static void SigHandlerSIGUSR2(int signum, siginfo_t *info, void *ucontext)
{
    sleep(1);
    printf("ping!\n");
    kill(info->si_pid, SIGUSR1);

    (void)signum;
    (void)ucontext;
}

int main(int argc, char *argv[])
{
    struct sigaction sigusr2_action = {0};
    int child_status = 0;
    int counter = 0;
    pid_t child_pid = 0;

    sigusr2_action.sa_flags = SA_SIGINFO;
    sigusr2_action.sa_sigaction = SigHandlerSIGUSR2;
    sigaction(SIGUSR2, &sigusr2_action, NULL);

    child_pid = fork();
    if(0 == child_pid)
    {
        char *child_argv[] = {"./pong.out", NULL};
        
        execvp("./pong.out", child_argv);
        fprintf (stderr, "an error occurred in execvp\n");
        abort();
    }

    sleep(1);
    printf("ping!\n");
    kill(child_pid, SIGUSR1);

    while(1)
    {
        pause();
    }

    (void)argc;
	(void)argv;

    return 0;
}
