#define _POSIX_C_SOURCE 199309L

#include <signal.h>     /*  kill(), sigaction(), struct sigaction   */
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

atomic_size_t counter;
int exit_status = 1;

static void SigHandlerSIGUSR1(int signum, siginfo_t *info, void *ucontext)
{
    if((pid_t)(info->si_int) == info->si_pid)
    {
        atomic_fetch_add(&counter, 1);
    }
    (void)signum;
    (void)ucontext;
}

static void UserProcSigHandlerSIGUSR2(int signum, siginfo_t *info, 
                                        void *ucontext)
{
    if((pid_t)info->si_int == info->si_pid)
    {
        exit_status = 0;
    }
    (void)signum;
    (void)ucontext;
}


static void InitSigHandlers()
{
    struct sigaction sigusr1_action = {0};
    struct sigaction sigusr2_action = {0};

    sigusr1_action.sa_flags = SA_SIGINFO;
    sigusr1_action.sa_sigaction = SigHandlerSIGUSR1;
    sigaction(SIGUSR1, &sigusr1_action, NULL);

    sigusr2_action.sa_flags = SA_SIGINFO;
    sigusr2_action.sa_sigaction = UserProcSigHandlerSIGUSR2;
    sigaction(SIGUSR2, &sigusr2_action, NULL);
}

int main()
{
    siginfo_t info = {0};
    struct timespec timeout = {0};
    sigset_t signal_from_partner = {0};
    static union sigval my_sig;
    pid_t partner_pid = getppid();
    int i = 0;
    int j = 2;
    int k = 0;
    int counter_check = 0;

    InitSigHandlers();
    my_sig.sival_int = getpid();
    timeout.tv_sec = 5;
    sigemptyset(&signal_from_partner);/*!!!*/
    sigaddset(&signal_from_partner, SIGUSR2);/*!!!*/

    do
    {
        for(i = 0; 5 > i; ++i)
        {
            sigqueue(partner_pid, SIGUSR1, my_sig);
            sleep(1);
        }
        counter_check = atomic_exchange(&counter, 0);
        ++k;
    }
    while(0 < j-- && 0 < counter_check);

    sigqueue(partner_pid, SIGUSR2, my_sig);
    
    sigtimedwait(&signal_from_partner, &info, &timeout);
    
    return (exit_status != 0 && 0 < counter_check);
}