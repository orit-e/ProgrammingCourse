/******************************************************************************
 											
				File name:			prod_cons_ex_1.c			
				Written by:     	Orit			
				Reviewed by:    	Guy B
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf()                            */
#include <pthread.h>    /*  pthread_create(), pthread_join()    */
#include <stdatomic.h>  /*  atomic_load(), atomic_fetch_add(),
                            atomic_store()                      */

#define ARR_SIZE 1000
#define ITER 5

enum PROD_CONS_MODES
{
    WRITE_MODE,
    READ_MODE
};

enum PROD_CONS_STATUS
{
    PROD_CONS_SUCCESS,
    PROD_CONS_FAIL
};

atomic_int array[ARR_SIZE];
atomic_int flag = WRITE_MODE;

static void *Producer(void *);
static void *Consumer(void *);

int main(int argc, char *argv[])
{
    pthread_t thread_id[2] = {0};

    if(0 != pthread_create(&thread_id[0], NULL, Producer, NULL))
    {
        return PROD_CONS_FAIL;
    }
    if(0 != pthread_create(&thread_id[1], NULL, Consumer, NULL))
    {
        return PROD_CONS_FAIL;
    }

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    (void)argc;
    (void)argv;

    return PROD_CONS_SUCCESS;
}

static void *Producer(void *ignored)
{
    int i = 0;
    int j = 0;

    while(ITER > j++)
    {
        while (atomic_load(&flag) == READ_MODE)
        {
            ;
        }
        for(i = 0; i < ARR_SIZE; ++i)
        {
            atomic_fetch_add(&array[i], 1);
        }
        atomic_store(&flag, READ_MODE);
    }

    (void)ignored;

    return NULL;
}

static void *Consumer(void *ignored)
{
    int i = 0;
    int j = 0;
    int sum = 0;
    int element_val = 0;

    while(ITER > j++)
    {
        while (atomic_load(&flag) == WRITE_MODE)
        {
            ;
        }
        for(i = 0; i < ARR_SIZE; ++i)
        {
            element_val = atomic_load(&array[i]);
            sum += element_val;
        }
        printf("Sum of all elements in the array is equal to: %d\n", sum);
        printf("Number of iteration: %d\n", j);
        sum = 0;
        atomic_store(&flag, WRITE_MODE);
    }

    (void)ignored;
    
    return NULL;
}