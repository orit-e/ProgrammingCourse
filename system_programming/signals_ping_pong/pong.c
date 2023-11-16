#define _POSIX_C_SOURCE 199309L

#include <signal.h>     /*  kill(), sigaction(), struct sigaction   */
#include <sys/types.h>  /*  kill(),                                 */
#include <unistd.h>     /*  pause(), sleep()                        */
#include <stdio.h>      /*  printf()                                */

static void SigHandlerSIGUSR1(int signum, siginfo_t *info, void *ucontext)
{
    sleep(1);
    printf("pong!\n");
    kill(info->si_pid, SIGUSR2);

    (void)signum;
    (void)ucontext;
}

int main(int argc, char *argv[])
{
    struct sigaction sigusr1_action = {0};
    int counter = 0;

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
