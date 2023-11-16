/******************************************************************************
 											
				File name:			prod_cons_ex_5.c			
				Written by:     	Orit			
				Reviewed by:    	Guy B
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf()                                        */
#include <pthread.h>    /*  pthread_create(), pthread_join(),
                            pthread_mutex_init(), pthread_mutex_destroy(),
                            pthread_mutex_lock(), pthread_mutex_unlock(),
                            pthread_cond_init(), pthread_cond_destroy(),
                            pthread_cond_brodcast(), pthread_cond_wait()    */
#include <semaphore.h>  /*  sem_init(), sem_destroy(), sem_wait(), 
                            sem_post()                                      */
#include <assert.h>     /*  assert()                                        */

#define THREAD_NUM 8
#define MAX_LOOP 5

enum PROD_CONS_STATUS
{
    PROD_CONS_SUCCESS,
    INIT_SYNC_TOOLS_ERROR_MUTEX,
    INIT_SYNC_TOOLS_ERROR_SEM,
    INIT_SYNC_TOOLS_ERROR_COND,
    PROD_CONS_CREATE_PTHREAD_FAIL
};

int global_massage;
int global_version;

sem_t semaphore;
pthread_mutex_t mutex;
pthread_cond_t cond;

static void *Producer(void *ignored);
static void *Consumer(void *ptr_num);
static int InitSyncToolsIMP(void);

int main(int argc, char *argv[])
{
    pthread_t thread_id[THREAD_NUM] = {0};
    size_t i = 0;
    int init_sync_status = 0;

    init_sync_status = InitSyncToolsIMP();
    if(0 != init_sync_status)
    {
        fprintf(stderr, "Initializing sync tools failed.\n");
        if(INIT_SYNC_TOOLS_ERROR_COND == init_sync_status)
        {
            pthread_mutex_destroy(&mutex);
        }
        if(INIT_SYNC_TOOLS_ERROR_MUTEX == init_sync_status)
        {
            sem_destroy(&semaphore);
        }

        return init_sync_status;
    }

    if(0 != pthread_create(&thread_id[0], NULL, Producer, NULL))
    {
        fprintf(stderr, "Insufficient resources for exercise.\n");

        pthread_mutex_destroy(&mutex);
        sem_destroy(&semaphore);
        pthread_cond_destroy(&cond);

        return PROD_CONS_CREATE_PTHREAD_FAIL;
    }
    for(i = 1; i < THREAD_NUM; ++i)
    {
        if(0 != pthread_create(&thread_id[i], NULL, Consumer, (void *)i))
        {
            fprintf(stderr, "Insufficient resources for exercise.\n");

            pthread_cancel(thread_id[0]);
            pthread_mutex_destroy(&mutex);
            sem_destroy(&semaphore);
            pthread_cond_destroy(&cond);

            return PROD_CONS_CREATE_PTHREAD_FAIL;
        }
    }

    for(i = 0; i < THREAD_NUM; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
    pthread_cond_destroy(&cond);

    (void)argc;
    (void)argv;

    return PROD_CONS_SUCCESS;
}

static void *Producer(void *ignored)
{
    int version_local = 0;
    int i = 0;
    int j = 0;

    while(MAX_LOOP > i++)
    {
        ++version_local;
        for(j = 1; THREAD_NUM > j; ++j)
        {
            while(0 != sem_wait(&semaphore));
        }
        ++global_massage;
        global_version = version_local;

        if(0 != pthread_mutex_lock(&mutex))
        {
           fprintf(stderr, "mutex error.\n");
            return NULL;
        }
        printf("Go!\n");
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    (void)ignored;

    return NULL;
}

static void *Consumer(void *ptr_num)
{
    int i = 0;
    int local_version = 0;
    int massage_local = 0;
    size_t num_of_thread = (size_t)ptr_num;

    while(MAX_LOOP > i++)
    {
        if(0 != pthread_mutex_lock(&mutex))
        {
            fprintf(stderr, "mutex error.\n");

            return NULL;
        }
        sem_post(&semaphore);
        while(local_version == global_version)
        {
            if(0 != pthread_cond_wait(&cond, &mutex))
            {
                fprintf(stderr, "conditional variable error.\n");
                pthread_mutex_unlock(&mutex);

                return NULL;
            }
        }
        local_version = global_version;
        massage_local = global_massage;
        pthread_mutex_unlock(&mutex);
        printf("massage num: %d, thread num: %lu, loop num: %d\n", 
                massage_local, num_of_thread, i);
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

    if(0 != pthread_cond_init(&cond, NULL))
    {
        return INIT_SYNC_TOOLS_ERROR_COND;
    }

    return PROD_CONS_SUCCESS;
}