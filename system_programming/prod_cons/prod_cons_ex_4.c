/******************************************************************************
 											
				File name:			prod_cons_ex_4.c			
				Written by:     	Orit			
				Reviewed by:    	Guy B
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf()                                        */
#include <pthread.h>    /*  pthread_create(), pthread_join(),
                            pthread_mutex_init(), pthread_mutex_destroy(),
                            pthread_mutex_lock(), pthread_mutex_unlock()    */
#include <assert.h>     /*  assert()                                        */

#include "circular_queue.h"

#define THREAD_NUM 8
#define MAX_LOOP 10
#define CIRC_Q_CAPACITY 10

enum PROD_CONS_STATUS
{
    PROD_CONS_SUCCESS,
    PROD_CONS_MUTEX_FAIL,
    PROD_CONS_CREATE_CIRC_Q_FAIL,
    PROD_CONS_CREATE_PTHREAD_FAIL
};

typedef struct thread_arg
{
    circ_q_t *circ_q;
    int thread_num;
}thread_arg_t;

pthread_mutex_t mutex_enq;
pthread_mutex_t mutex_deq;
int write_index;
int read_index;

static void *Producer(void *ptr_circ_q);
static void *Consumer(void *ptr_circ_q);

int main(int argc, char *argv[])
{
    circ_q_t *circ_q = NULL;
    pthread_t thread_id[THREAD_NUM] = {0};
    thread_arg_t thread_arg[THREAD_NUM] = {0};
    int i = 0;

    if(0 != pthread_mutex_init(&mutex_enq, NULL) ||
        0 != pthread_mutex_init(&mutex_deq, NULL))
    {
        fprintf(stderr, "Initializing sync tools failed.\n");

        return PROD_CONS_MUTEX_FAIL;
    }

    circ_q = CircQCreate(CIRC_Q_CAPACITY);
    if(NULL == circ_q)
    {
        fprintf(stderr, "creating slist failed.\n");
        pthread_mutex_destroy(&mutex_enq);
        pthread_mutex_destroy(&mutex_deq);

        return PROD_CONS_CREATE_CIRC_Q_FAIL;
    }

    for(i = 0; i < THREAD_NUM; ++i)
    {
        thread_arg[i].circ_q = circ_q;
        thread_arg[i].thread_num = i;
    }
    for(i = 0; i < THREAD_NUM / 2; ++i)
    {
        if(0 != pthread_create(&thread_id[i], NULL, Producer, 
                                (void *)&thread_arg[i]) ||
            0 != pthread_create(&thread_id[i + THREAD_NUM / 2], NULL, Consumer, 
                                (void *)&thread_arg[i + THREAD_NUM / 2]))
        {
            fprintf(stderr, "Insufficient resources for exercise.\n");
            CircQDestroy(circ_q);
            pthread_mutex_destroy(&mutex_enq);
            pthread_mutex_destroy(&mutex_deq);

            return PROD_CONS_CREATE_PTHREAD_FAIL;
        }
    }

    for(i = 0; i < THREAD_NUM; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
    CircQDestroy(circ_q);
    pthread_mutex_destroy(&mutex_enq);
    pthread_mutex_destroy(&mutex_deq);

    (void)argc;
    (void)argv;

    return PROD_CONS_SUCCESS;
}

static void *Producer(void *ptr_thread_arg)
{
    thread_arg_t *thread_arg = NULL;
    int i = 0;

    assert(NULL != ptr_thread_arg);

    thread_arg = (thread_arg_t *)ptr_thread_arg;

    while(MAX_LOOP > i++)
    {
        if(0 != pthread_mutex_lock(&mutex_enq))
        {
            fprintf(stderr, "mutex error.\n");

            return NULL;
        }
        if(CIRC_Q_CAPACITY == write_index - read_index)
        {
            if(0 != pthread_mutex_lock(&mutex_deq))
            {
                fprintf(stderr, "mutex error.\n");

                return NULL;
            }
            CircQDeQueue(thread_arg->circ_q);
            ++read_index;
            pthread_mutex_unlock(&mutex_deq);
        }
        CircQEnQueue(thread_arg->circ_q, i);
        ++write_index;
        pthread_mutex_unlock(&mutex_enq);
        printf("In! Thread num: %d! Loop: %d!\n", thread_arg->thread_num, i);
    }

    return NULL;
}

static void *Consumer(void *ptr_thread_arg)
{
    thread_arg_t *thread_arg = NULL;
    size_t num = 0;
    int temp_wr = 0;
    int i = 0;

    assert(NULL != ptr_thread_arg);

    thread_arg = (thread_arg_t *)ptr_thread_arg;

    while(MAX_LOOP > i++)
    {
        if(0 != pthread_mutex_lock(&mutex_deq))
        {
            fprintf(stderr, "mutex error.\n");

            return NULL;
        }
        temp_wr = write_index - read_index;
        if(0 < temp_wr)
        {
            num = CircQDeQueue(thread_arg->circ_q);
            ++read_index;
            pthread_mutex_unlock(&mutex_deq);
            printf("Out! num exerted from q: %lu! Thread num: %d! Loop: %d!\n", 
                    num, thread_arg->thread_num, i);
        }
        else
        {
            pthread_mutex_unlock(&mutex_deq);
            printf("Nothing in queue... Thread num: %d! Loop: %d!\n", 
                    thread_arg->thread_num, i);
        }
    }

    return NULL;
}