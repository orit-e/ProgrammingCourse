/******************************************************************************
 											
				File name:			prod_cons_ex_3_2.c			
				Written by:     	Orit			
				Reviewed by:    	Guy B
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf()                                        */
#include <pthread.h>    /*  pthread_create(), pthread_join(),
                            pthread_mutex_init(), pthread_mutex_destroy(),
                            pthread_mutex_lock(), pthread_mutex_unlock()    */
#include <semaphore.h>  /*  sem_init(), sem_destroy(), sem_wait(), 
                            sem_post()                                      */
#include <assert.h>     /*  assert()                                        */

#include "circular_queue.h"

#define THREAD_NUM 8
#define MAX_LOOP 5
#define CIRC_Q_CAPACITY 10

enum PROD_CONS_STATUS
{
    PROD_CONS_SUCCESS,
    INIT_SYNC_TOOLS_ERROR_MUTEX,
    INIT_SYNC_TOOLS_ERROR_SEM,
    PROD_CONS_CREATE_CIRC_Q_FAIL,
    PROD_CONS_CREATE_PTHREAD_FAIL
};

sem_t semaphore_enq;
sem_t semaphore_deq;
pthread_mutex_t mutex_enq;
pthread_mutex_t mutex_deq;

static void *Producer(void *ptr_circ_q);
static void *Consumer(void *ptr_circ_q);
static int InitSyncToolsIMP(void);

int main(int argc, char *argv[])
{
    circ_q_t *circ_q = NULL;
    pthread_t thread_id[THREAD_NUM] = {0};
    int i = 0;
    int init_sync_status = 0;

    init_sync_status = InitSyncToolsIMP();
    if(0 != init_sync_status)
    {
        fprintf(stderr, "Initializing sync tools failed.\n");
        if(INIT_SYNC_TOOLS_ERROR_MUTEX == init_sync_status)
        {
            pthread_mutex_destroy(&mutex_enq);
            pthread_mutex_destroy(&mutex_deq);
        }
        sem_destroy(&semaphore_enq);
        sem_destroy(&semaphore_deq);

        return init_sync_status;
    }

    circ_q = CircQCreate(CIRC_Q_CAPACITY);
    if(NULL == circ_q)
    {
        fprintf(stderr, "Creating circular queue failed.\n");
        pthread_mutex_destroy(&mutex_enq);
        pthread_mutex_destroy(&mutex_deq);
        sem_destroy(&semaphore_enq);
        sem_destroy(&semaphore_deq);

        return PROD_CONS_CREATE_CIRC_Q_FAIL;
    }

    for(i = 0; i < THREAD_NUM / 2; ++i)
    {
        if(0 != pthread_create(&thread_id[i], NULL, Producer, (void *)circ_q) ||
            0 != pthread_create(&thread_id[i + THREAD_NUM / 2], NULL, Consumer, 
                                (void *)circ_q))
        {
            fprintf(stderr, "Insufficient resources for exercise.\n");
            CircQDestroy(circ_q);
            pthread_mutex_destroy(&mutex_enq);
            pthread_mutex_destroy(&mutex_deq);
            sem_destroy(&semaphore_enq);
            sem_destroy(&semaphore_deq);

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
    sem_destroy(&semaphore_enq);
    sem_destroy(&semaphore_deq);

    (void)argc;
    (void)argv;

    return PROD_CONS_SUCCESS;
}

static void *Producer(void *ptr_circ_q)
{
    circ_q_t *circ_q = NULL;
    int i = 0;

    assert(NULL != ptr_circ_q);

    circ_q = (circ_q_t *)ptr_circ_q;

    while(MAX_LOOP > i++)
    {
        while(0 != sem_wait(&semaphore_enq));
        if(0 != pthread_mutex_lock(&mutex_enq))
        {
            fprintf(stderr, "mutex error.\n");

            return NULL;
        }
        CircQEnQueue(circ_q, i);
        printf("In! num: %d!\n", i);
        sem_post(&semaphore_deq);
        pthread_mutex_unlock(&mutex_enq);
    }

    return NULL;
}

static void *Consumer(void *ptr_circ_q)
{
    circ_q_t *circ_q = NULL;
    size_t num = 0;
    int i = 0;

    assert(NULL != ptr_circ_q);

    circ_q = (circ_q_t *)ptr_circ_q;

    while(MAX_LOOP > i++)
    {
        while(0 != sem_wait(&semaphore_deq));
        if(0 != pthread_mutex_lock(&mutex_deq))
        {
            fprintf(stderr, "mutex error.\n");

            return NULL;
        }
        num = CircQDeQueue(circ_q);
        printf("Out! num: %lu! loop num: %d!\n", num, i);
        sem_post(&semaphore_enq);
        pthread_mutex_unlock(&mutex_deq);
    }

    return NULL;
}

static int InitSyncToolsIMP()
{
    if(0 != sem_init(&semaphore_enq, 0, CIRC_Q_CAPACITY) ||
        0 != sem_init(&semaphore_deq, 0, 0))
    {
        return INIT_SYNC_TOOLS_ERROR_SEM;
    }
    
    if(0 != pthread_mutex_init(&mutex_enq, NULL) ||
        0 != pthread_mutex_init(&mutex_deq, NULL))
    {
        return INIT_SYNC_TOOLS_ERROR_MUTEX;
    }

    return PROD_CONS_SUCCESS;
}