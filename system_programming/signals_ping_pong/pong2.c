#define _POSIX_C_SOURCE 199309L

#include <signal.h>     /*  kill(), sigaction(), struct sigaction   */
#include <sys/types.h>  /*  kill()                                  */
#include <unistd.h>     /*  pause(), sleep()                        */
#include <stdio.h>      /*  printf()                                */
#include <stdlib.h>     /*  atoi()                                  */

static void SigHandlerSIGUSR2(int signum, siginfo_t *info, void *ucontext)
{
    sleep(1);
    printf("pong!\n");
    kill(info->si_pid, SIGUSR1);

    (void)signum;
    (void)ucontext;
}

int main(int argc, char *argv[])
{
    struct sigaction sigusr2_action = {0};
    pid_t ping_pid = 0;

    sigusr2_action.sa_flags = SA_SIGINFO;
    sigusr2_action.sa_sigaction = SigHandlerSIGUSR2;
    sigaction(SIGUSR2, &sigusr2_action, NULL);

    ping_pid = atoi(argv[1]);
    kill(ping_pid, SIGUSR1);
    while(1)
    {
        pause();
    }
    
    (void)argc;

    return 0;
}