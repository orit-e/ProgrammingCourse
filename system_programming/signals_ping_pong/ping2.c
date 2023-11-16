#define _POSIX_C_SOURCE 199309L

#include <signal.h>     /*  kill(), sigaction(), struct sigaction   */
#include <sys/types.h>  /*  kill(), getpid()                        */
#include <unistd.h>     /*  sleep(), pause(), getpid()              */
#include <stdio.h>      /*  printf(), sprintf()                     */
#include <sys/wait.h>   /*  wait()                                  */
#include <stdlib.h>     /*  system()                                */

static void SigHandlerSIGUSR1(int signum, siginfo_t *info, void *ucontext)
{
    sleep(1);
    printf("ping!\n");
    kill(info->si_pid, SIGUSR2);

    (void)signum;
    (void)ucontext;
}

int main(int argc, char *argv[])
{
    struct sigaction sigusr1_action = {0};

    sigusr1_action.sa_flags = SA_SIGINFO;
    sigusr1_action.sa_sigaction = SigHandlerSIGUSR1;
    sigaction(SIGUSR1, &sigusr1_action, NULL);

    while(1)
    {
        pause();
    }

    (void)argc;
    (void)argv;

    return 0;
}
