#include <stdio.h>      /*  printf(), size_t                */
#include <pthread.h>    /*  pthread_create(), pthread_t     */
#include <unistd.h>     /*  sleep()                         */

#define SIZE 100000

int arr[SIZE];

void *InsertValueToArrMember(void *i);

int main(int argc, char *argv[])
{
    pthread_t thread_id[SIZE] = {0};
    size_t i = 0;
    
    for(i = 0; i < SIZE; ++i)
    {
        pthread_create(&thread_id[i], NULL, &InsertValueToArrMember, (void *)i);
    }
    
    sleep(10);

    for(i = 0; i < SIZE; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    (void)argc;
	(void)argv;

    return 0;
}

void *InsertValueToArrMember(void *i)
{
    size_t index = (size_t)i;

    arr[index] = index;
    
    return NULL;
}
