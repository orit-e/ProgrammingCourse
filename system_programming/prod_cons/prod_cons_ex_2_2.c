/******************************************************************************
 											
				File name:			prod_cons_ex_2_2.c			
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

#include "../../ds/slist/slist.h"

#define THREAD_NUM 8
#define MAX_LOOP 5

enum PROD_CONS_STATUS
{
    PROD_CONS_SUCCESS,
    INIT_SYNC_TOOLS_ERROR_MUTEX,
    INIT_SYNC_TOOLS_ERROR_SEM,
    PROD_CONS_CREATE_SLIST_FAIL,
    PROD_CONS_CREATE_PTHREAD_FAIL
};

sem_t semaphore;
pthread_mutex_t mutex;

static void *Producer(void *ptr_slist);
static void *Consumer(void *ptr_slist);
static int InitSyncToolsIMP(void);

int main(int argc, char *argv[])
{
    slist_t *slist = NULL;
    pthread_t thread_id[THREAD_NUM] = {0};
    int i = 0;
    int init_sync_status = 0;

    init_sync_status = InitSyncToolsIMP();
    if(0 != init_sync_status)
    {
        fprintf(stderr, "Initializing sync tools failed.\n");
        if(INIT_SYNC_TOOLS_ERROR_MUTEX == init_sync_status)
        {
            sem_destroy(&semaphore);
        }
        
        return init_sync_status;
    }

    slist = SListCreate();
    if(NULL == slist)
    {
        fprintf(stderr, "creating slist failed.\n");
        pthread_mutex_destroy(&mutex);
        sem_destroy(&semaphore);

        return PROD_CONS_CREATE_SLIST_FAIL;
    }

    for(i = 0; i < THREAD_NUM / 2; ++i)
    {
        if(0 != pthread_create(&thread_id[i], NULL, Producer, (void *)slist) ||
            0 != pthread_create(&thread_id[i + THREAD_NUM / 2], NULL, Consumer, 
                                (void *)slist))
        {
            fprintf(stderr, "Insufficient resources for exercise.\n");
            SListDestroy(slist);
            pthread_mutex_destroy(&mutex);
            sem_destroy(&semaphore);

            return PROD_CONS_CREATE_PTHREAD_FAIL;
        }
    }

    for(i = 0; i < THREAD_NUM; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
    SListDestroy(slist);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);

    (void)argc;
    (void)argv;

    return PROD_CONS_SUCCESS;
}

static void *Producer(void *ptr_slist)
{
    slist_t *slist = NULL;
    iterator_t iter = NULL;
    int i = 0;

    assert(NULL != ptr_slist);

    slist = (slist_t *)ptr_slist;

    while(MAX_LOOP > i++)
    {
        if(0 != pthread_mutex_lock(&mutex))
        {
            printf("mutex error.\n");
            return NULL;
        }

        iter = SListEnd(slist);
        iter = SListInsert(iter, (void *)1);
        if(SListEnd(slist) == iter)
        {
            printf("slist insert error\n");
            return NULL;
        }
        pthread_mutex_unlock(&mutex);

        sem_post(&semaphore);
    }

    return NULL;
}

static void *Consumer(void *ptr_slist)
{
    slist_t *slist = NULL;
    iterator_t iter = NULL;
    size_t num = 0;
    int i = 0;

    assert(NULL != ptr_slist);

    slist = (slist_t *)ptr_slist;

    while(MAX_LOOP > i++)
    {
        while(0 != sem_wait(&semaphore));

        if(0 != pthread_mutex_lock(&mutex))
        {
            printf("mutex error.\n");

            return NULL;
        }
        iter = SListBegin(slist);
        num = (size_t)SListGetData(iter);
        SListRemove(iter);
        printf("%lu! %d!\n", num, i);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

static int InitSyncToolsIMP()
{
    if(0 != sem_init(&semaphore, 0, 0))
    {
        return INIT_SYNC_TOOLS_ERROR_SEM;
    }
    
    if(0 != pthread_mutex_init(&mutex, NULL))
    {
        return INIT_SYNC_TOOLS_ERROR_MUTEX;
    }

    return PROD_CONS_SUCCESS;
}