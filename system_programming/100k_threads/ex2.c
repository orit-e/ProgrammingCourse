#include <stdio.h>      /*  printf(), size_t                */
#include <pthread.h>    /*  pthread_create(), pthread_t     */
#include <unistd.h>     /*  sleep()                         */

#define CREATE_SUCCESS 0
#define SIZE 100000

int arr[SIZE];

void *InsertValueToArrMember(void *i);

int main(int argc, char *argv[])
{
    pthread_t thread_id[SIZE] = {0};
    size_t i = 0;
    size_t j = 0;
    int status = CREATE_SUCCESS;
    
    for(i = 0; i < SIZE; ++i)
    {
        status = pthread_create(&thread_id[i], NULL, &InsertValueToArrMember, 
                                (void *)i);
        if(CREATE_SUCCESS != status)
        {
            --i;
            pthread_join(thread_id[j], NULL);
            ++j;
        }
        
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
