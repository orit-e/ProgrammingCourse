/******************************************************************************
 											
				File name:			prod_cons_ex_2_1.c			
				Written by:     	Orit			
				Reviewed by:    	Guy B
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf(), fprintf()                             */
#include <pthread.h>    /*  pthread_create(), pthread_join(),
                            pthread_mutex_init(), pthread_mutex_destroy(),
                            pthread_mutex_lock(), pthread_mutex_unlock()    */
#include <assert.h>     /*  assert()                                        */

#include "../../ds/slist/slist.h"

#define THREAD_NUM 8
#define MAX_LOOP 5

enum PROD_CONS_STATUS
{
    PROD_CONS_SUCCESS,
    PROD_CONS_FAIL
};

pthread_mutex_t mutex;

static void *Producer(void *ptr_slist);
static void *Consumer(void *ptr_slist);

int main(int argc, char *argv[])
{
    slist_t *slist = NULL;
    pthread_t thread_id[THREAD_NUM] = {0};
    int i = 0;

    if(0 != pthread_mutex_init(&mutex, NULL))
    {
        fprintf(stderr, "Initializing sync tools failed.\n");

        return PROD_CONS_FAIL;
    }

    slist = SListCreate();
    if(NULL == slist)
    {
        fprintf(stderr, "creating slist failed.\n");
        pthread_mutex_destroy(&mutex);

        return PROD_CONS_FAIL;
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

            return PROD_CONS_FAIL;
        }
    }

    for(i = 0; i < THREAD_NUM; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
    SListDestroy(slist);
    pthread_mutex_destroy(&mutex);

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
        pthread_mutex_lock(&mutex);

        iter = SListEnd(slist);
        iter = SListInsert(iter, (void *)1);
        if(SListEnd(slist) == iter)
        {
            fprintf(stderr, "slist insert error\n");

            return NULL;
        }

        pthread_mutex_unlock(&mutex);
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
        if(0 != pthread_mutex_lock(&mutex))
        {
            fprintf(stderr, "mutex error.\n");
            
            return NULL;
        }

        if(1 != SListIsEmptyList(slist))
        {
            iter = SListBegin(slist);
            num = (size_t)SListGetData(iter);
            SListRemove(iter);
            printf("num (data from slist): %lu! loop num: %d!\n", num, i);
        }
        else
        {
            printf("Nothing to print... loop num: %d\n", i);
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
