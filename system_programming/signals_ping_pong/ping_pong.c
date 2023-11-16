#define _POSIX_SOURCE

#include <signal.h>     /*  kill(), sigaction(), struct sigaction   */
#include <sys/types.h>  /*  kill(), fork(), wait()                  */
#include <unistd.h>     /*  fork(), sleep(), pause()                */
#include <stdio.h>      /*  printf()                                */
#include <sys/wait.h>   /*  wait()                                  */

static void SigHandlerSIGUSR(int signum)
{
    sleep(1);
    (void)signum;
}

int main(int argc, char *argv[])
{
    struct sigaction sigusr_action = {0};
    int child_status = 0;
    pid_t child_pid = 0;

    sigusr_action.sa_handler = SigHandlerSIGUSR;
    sigaction(SIGUSR1, &sigusr_action, NULL);
    sigaction(SIGUSR2, &sigusr_action, NULL);
    child_pid = fork();
    if(0 == child_pid)
    {
        int counter = 0;

        while(5 > counter++)
        {
            pause();
            printf("pong!\n");
            kill(getppid(), SIGUSR2);
        } 
    }
    else
    {
        int counter = 0;

        sleep(1);
        while(5 > counter++)
        {
            printf("ping!\n");
            kill(child_pid, SIGUSR1);
            pause();
        }
    }
    
    wait(&child_status);

    (void)argc;
	(void)argv;

    return 0;
}

