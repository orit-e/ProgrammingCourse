#include <sys/types.h>  /*  ftok()                                  */
#include <sys/ipc.h>    /*  ftok()                                  */
#include <sys/sem.h>    /*  semget(), semctl(), semop()             */
#include <errno.h>      /*  errno                                   */
#include <stdio.h>      /*  printf(), fgets(), fopen(), fclose()    */
#include <stdlib.h>     /*  strtod()                                */
#include <strings.h>    /*  strncasecmp()                           */
#include <unistd.h>     /*  sleep()                                 */
#include <assert.h>     /*  assert()                                */

#define MAX_RETRIES 10
#define SOME_ERROR -1
#define SEM_MAN_SUCCESS 0

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

static int GetSemaphoreID(const char *pathname);
static int InitializeSemaphore(int sem_id);
static int ObtainSemaphoreID(key_t key);
static int IsSemReady(int sem_id);
static int CommandLoop(int sem_id);
static int CommandD(char *buffer, int sem_id);
static int CommandI(char *buffer, int sem_id);
static int CommandV(int sem_id);
static int SetSemVal(int sem_id, int num, int is_undo);

int main(int argc, char *argv[])
{
    FILE *ptr = NULL;
    char *sem_file_name = argv[1];
    int sem_id = 0;
    int status = 0;

    ptr = fopen(sem_file_name, "w");
    if(NULL == ptr)
    {
        printf("Was not able to open file - \"%s\"\n", sem_file_name);

        return SOME_ERROR;
    }
    fclose(ptr);

    sem_id = GetSemaphoreID(sem_file_name);

    if(0 > sem_id)
    {
        printf("No good! sem_id = %d\n", sem_id);
        return sem_id;
    }

    status = CommandLoop(sem_id);
    
    (void)argc;
    
    return status;
}


static int GetSemaphoreID(const char *pathname)
{
    key_t key = 0;
    int sem_id = 0;
    int status = 0;

    assert(NULL != pathname);

    key = ftok(pathname, 'c');
    if(SOME_ERROR == key)
    {
        return key;
    }

    sem_id = semget(key, 1, (IPC_CREAT | IPC_EXCL | 0666));
    if(0 <= sem_id)
    {
        status = InitializeSemaphore(sem_id);
        if(0 != status)
        {
            return SOME_ERROR;
        }
    }
    else if(EEXIST == errno)
    {
        sem_id = ObtainSemaphoreID(key);
    }

    return sem_id;
}

static int InitializeSemaphore(int sem_id)
{
    union semun argument;
    unsigned short int values[1] = {1};

    assert(0 <= sem_id);

    argument.array = values;

    return semctl(sem_id, 0, SETALL, argument);
}

static int ObtainSemaphoreID(key_t key)
{
    int sem_id = 0;

    assert(SOME_ERROR != key);

    sem_id = semget(key, 1, (IPC_CREAT | 0666));
    if(0 > sem_id)
    {
        return sem_id;
    }
    
    if(1 != IsSemReady(sem_id))
    {
        return SOME_ERROR;
    }
    
    return sem_id;
}

static int IsSemReady(int sem_id)
{
    int i = 0;
    union semun argument;

    assert(0 <= sem_id);

    do
    {
        sleep(1);
        semctl(sem_id, 0, IPC_STAT, argument);
    }    
    while(MAX_RETRIES > i++ && 0 != argument.buf->sem_otime);

    return (0 != argument.buf->sem_otime);
}

static int CommandLoop(int sem_id)
{
    int status = 0;
    char buffer[80] = {0};

    assert(0 <= sem_id);

    printf("Semaphore is ready.\n");
    printf("Please enter a command:\n");

    while('X' != buffer[0] && 0 == status)
    {
        fgets(buffer, 80, stdin);
        switch (buffer[0])
        {
            case 'D':
                status = CommandD(buffer, sem_id);
                break;
            case 'I':
                status = CommandI(buffer, sem_id);
                break;
            case 'V':
                status = CommandV(sem_id);
                break;
            case 'X':
                break;
            default:
                printf("Bad input, try again.\n");
                break;
        }
    }
        
    return status;
}

static int CommandD(char *buffer, int sem_id)
{
    char *ptr = NULL;
    int num = 0;
    int is_undo = 0;
    char test[] = "undo";

    assert(NULL != buffer);
    assert(0 <= sem_id);

    ptr = buffer + 2;
    num = -(int)strtod(ptr, &ptr);
    if('\n' != *ptr)
    {
        is_undo = (0 == strncasecmp(ptr + 1, test, 4));
    }
    
    return SetSemVal(sem_id, num, is_undo);
}

static int CommandI(char *buffer, int sem_id)
{
    char *ptr = NULL;
    int num = 0;
    int is_undo = 0;
    char test[] = "undo";

    assert(NULL != buffer);
    assert(0 <= sem_id);

    ptr = buffer + 2;    
    num = (int)strtod(ptr, &ptr);
    if('\n' != *ptr)
    {
        is_undo = (0 == strncasecmp(ptr + 1, test, 4));
    }
    
    return SetSemVal(sem_id, num, is_undo);
}

static int CommandV(int sem_id)
{
    int sem_val = 0;

    assert(0 <= sem_id);

    sem_val = semctl(sem_id, 0, GETVAL);
    if(SOME_ERROR == sem_val)
    {
        return SOME_ERROR;
    }
        
    printf("Semaphore number is: %d\n", sem_val);

    return SEM_MAN_SUCCESS;
}

static int SetSemVal(int sem_id, int num, int is_undo)
{
    struct sembuf sb[1] = {0};

    assert(0 <= sem_id);
    
    sb[0].sem_num = 0;
    sb[0].sem_op = num;
    sb[0].sem_flg = (1 == is_undo) ? SEM_UNDO : 0;

    return semop(sem_id, sb, 1);
}

