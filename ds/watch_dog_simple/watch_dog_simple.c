#include <stdio.h>      /*  printf(), fprintf() */
#include <stdlib.h>     /*  abort(), system()   */
#include <sys/types.h>  /*  fork(), wait()      */
#include <unistd.h>     /*  fork(), execvp()    */
#include <sys/wait.h>   /*  wait(), WIFEXITED() */

extern char **environ;

int WatchDog1(char *argv[]);
int WatchDog2(char *argv[]);
int WatchDog3(char *argv[]);
int WatchDogSystem(char *argv);
int Spawn1(char *argv[]);
int Spawn2(char *argv[]);

int main(int argc, char *argv[])
{
    /*int child_status = 0;
    int child_pid = 0;*/ 

    char *arg_list1[] = {"./dummy.out", "1", NULL};
    char *arg_list2[] = {"./dummy.out", "2", NULL};
    char *arg_list3[] = {"./dummy.out", "3", NULL};
    char arg_list4[] = "./dummy.out 4";

    WatchDog1(arg_list1);
    WatchDog2(arg_list2);
    WatchDog3(arg_list3);
    WatchDogSystem(arg_list4);

    (void)argc;
	(void)argv;

    return 0;
}

int WatchDog1(char *argv[])
{
    int child_status = 0;
    int counter = 0;

    do
    {
        Spawn1(argv);
        wait(&child_status);/* until child is terminated */
    }
    while (1 == WIFEXITED(child_status) && 5 > counter++);
    
    return 0;
}

int WatchDog2(char *argv[])
{
    int child_status = 0;
    int child_pid = 0;
    int counter = 0;

    do
    {
        child_pid = Spawn1(argv);
        waitpid(child_pid, &child_status, 0); /*0 == terminated state.*/
    }
    while (1 == WIFEXITED(child_status) && 5 > counter++);
    
    return 0;
}

int WatchDog3(char *argv[])
{
    int child_status = 0;
    int child_pid = 0;
    int counter = 0;

    do
    {
        child_pid = Spawn2(argv);
        waitpid(child_pid, &child_status, 0); /*0 == terminated state.*/
    }
    while (1 == WIFEXITED(child_status) && 5 > counter++);
    
    return 0;
}

int WatchDogSystem(char *argv)
{
    int child_status = 0;
    int counter = 0;

    do
    {
        child_status = system(argv);
    }
    while (1 == WIFEXITED(child_status) && -1 != child_status && 5 > counter++);
    
    return 0;
}

int Spawn1(char *argv[])
{
    pid_t child_pid;
    char buffer[80] = {0};

    child_pid = fork();
    sprintf (buffer, "cat /proc/%d/status | egrep State", child_pid);
    printf("%d:\n", child_pid);
    system(buffer);
    printf("\n");

    if (child_pid == 0)
    {
        char *program_name = argv[0];

        execvp(program_name, argv);
        fprintf (stderr, "an error occurred in execvp\n");
        abort();
    }

    return child_pid;
}

int Spawn2(char *argv[])
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == 0)
    {
        char *program_name = argv[0];

        execv(program_name, argv);
        fprintf (stderr, "An error occurred in execvp().\n");
        abort();
    }

    return child_pid;
}