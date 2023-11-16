/******************************************************************************
 											
                filename:			mt_cnt_sort.c			
                written by:     	Orit			
                reviewed by:    	Elia		
 											
 ******************************************************************************/
#include <stdio.h>      /*  perror()                                        */
#include <pthread.h>    /*  pthread_t, pthread_create(), pthread_join()     */
#include <stdlib.h>     /*  malloc(),free()                                 */
#include <string.h>     /*  memcpy()                                        */
#include <unistd.h>     /*  read()                                          */
#include <sys/types.h>  /*  open(), fstat()                                 */
#include <sys/stat.h>   /*  open(), fstat()                                 */
#include <fcntl.h>      /*  open(), fstat(), For O_*                        */
#include <assert.h>     /*  assert()                                        */
#include <errno.h>      /*  errno for perror()                              */

#define NUM_OF_THREADS          (8)
#define NUM_OF_DICT_COPIES      (4)
#define CHAR_MAX_NUM            (256)
#define SIZE_OF_CHAR            (1)
#define NUM_OF_ACTUAL_LETTERS   (26)

enum MT_CNT_SORT_STATUS
{
    MT_CNT_SORT_SUCCESS,
    FILE_OPERATION_ERROR,
    COPY_DICT_TO_BUFFER_ERROR
};

typedef struct thread_args
{
    char *split_buffer;
    size_t split_buffer_size;
    size_t counter[CHAR_MAX_NUM];
}thread_args_t;

static void *PartOfMultiDictionaryCount(void *ptr_args);
static int OpenDictAndFindSize(size_t *dict_size);
static char *MakeBufferAndCopyDictOnce(int dict_fd, size_t dict_size);
static void MakeMultiCopiesOfDictInBuf(char *memory_buffer, size_t dict_size);
static size_t FindSplitPartSize(char *split_memory_buffer, 
                                char *memory_buffer_end, 
                                size_t wanted_split_memory_size);
static void MergCountIMP(size_t *merge_counter, thread_args_t *thread_args);

int main()
{
    thread_args_t thread_args[NUM_OF_THREADS] = {0};
    char *memory_buffer = NULL;
    pthread_t thread_id[NUM_OF_THREADS] = {0};
    size_t dict_size = 0;
    size_t buffer_size = 0;
    size_t split_buffer_size = 0;
    size_t merge_counter[NUM_OF_ACTUAL_LETTERS] = {0};
    int dict_fd = 0;
    int i = 0;

    dict_fd = OpenDictAndFindSize(&dict_size);
    if (-1 == dict_fd)
    {
        return FILE_OPERATION_ERROR;
    }
    
    memory_buffer = MakeBufferAndCopyDictOnce(dict_fd, dict_size);
    if(NULL == memory_buffer)
    {
        return COPY_DICT_TO_BUFFER_ERROR;
    }
    
    MakeMultiCopiesOfDictInBuf(memory_buffer, dict_size);
    buffer_size = dict_size * NUM_OF_DICT_COPIES;
    split_buffer_size = (buffer_size / NUM_OF_THREADS);
    for(i = 0; NUM_OF_THREADS > i; ++i)
    {
        thread_args[i].split_buffer = memory_buffer + i * split_buffer_size;
        thread_args[i].split_buffer_size = 
                FindSplitPartSize(thread_args[i].split_buffer,
                                    memory_buffer + buffer_size,
                                    split_buffer_size);
        pthread_create(&thread_id[i], NULL, PartOfMultiDictionaryCount, 
                        (void *)&thread_args[i]);
    }
    
    for(i = 0; NUM_OF_THREADS > i; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
    free(memory_buffer);

    MergCountIMP(merge_counter, thread_args);
    for(i = 0; NUM_OF_ACTUAL_LETTERS > i; ++i)
    {
        printf("Letter %c: %lu times.\n", (char)(i + 'a') , merge_counter[i]);
    }
    
    return MT_CNT_SORT_SUCCESS;
}

static void *PartOfMultiDictionaryCount(void *ptr_args)
{
    thread_args_t *args = {0};
    size_t i = 0;

    assert(NULL != ptr_args);

    args = (thread_args_t *)ptr_args; 
    for(i = 0; args->split_buffer_size > i; ++i)
    {
        args->counter[(unsigned char)args->split_buffer[i]] += 1;
    }
    
    return NULL;
}

static int OpenDictAndFindSize(size_t *dict_size)
{
    struct stat dict_stat = {0};
    int dict_fd = 0;
    int status = 0;

    assert(NULL != dict_size);

    dict_fd = open("/usr/share/dict/words", O_RDONLY);
    if (-1 == dict_fd)
    {
        perror("open() failed");

        return dict_fd;
    }

    status = fstat(dict_fd, &dict_stat);
    if(-1 == status)
    {
        perror("fstat failed");
        close(dict_fd);

        return status;
    }

    *dict_size = dict_stat.st_size;

    return dict_fd;
}

static char *MakeBufferAndCopyDictOnce(int dict_fd, size_t dict_size)
{
    char *memory_buffer = NULL;
    ssize_t status = 0;

    memory_buffer = (char *)malloc(dict_size * NUM_OF_DICT_COPIES);
    if(NULL == memory_buffer)
    {
        perror("malloc failed");
        close(dict_fd);

        return NULL;
    }

    status = read(dict_fd, (void *)memory_buffer, dict_size);
    if(-1 == status)
    {
        perror("fread failed");
        close(dict_fd);
        free(memory_buffer);

        return NULL;
    }

    close(dict_fd);

    return memory_buffer;
}

static void MakeMultiCopiesOfDictInBuf(char *memory_buffer, size_t dict_size)
{
    int i = 0;

    assert(NULL != memory_buffer);
    assert(0 < dict_size);
    
    for(i = 1; NUM_OF_DICT_COPIES > i; ++i)
    {
        memcpy((void *)(memory_buffer + (i * dict_size)), memory_buffer, 
                dict_size);
    }
}

static size_t FindSplitPartSize(char *split_memory_buffer, 
                                char *memory_buffer_end, 
                                size_t wanted_split_memory_size)
{
    assert(NULL != split_memory_buffer);
    assert(NULL != memory_buffer_end);
    
    return (((size_t)(memory_buffer_end - split_memory_buffer) < 
                wanted_split_memory_size)                           ?
            (size_t)(memory_buffer_end - split_memory_buffer)       :
            wanted_split_memory_size);
}

static void MergCountIMP(size_t *merge_counter, thread_args_t *thread_args)
{
    int i = 0;
    int j = 0;

    for(i = 0; NUM_OF_THREADS > i; ++i)
    {
        for(j = 0; NUM_OF_ACTUAL_LETTERS > j; ++j)
        {
            merge_counter[j] += thread_args[i].counter[j + 'A'] +
                                thread_args[i].counter[j + 'a'];
        }
    }
}
