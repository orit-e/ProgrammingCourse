/******************************************************************************
 											
                filename:			mt_shuffle_sort.c			
                written by:     	Orit			
                reviewed by:    	Elia		
 											
 ******************************************************************************/
#include <stdio.h>      /*  perror()                                        */
#include <pthread.h>    /*  pthread_t, pthread_create(), pthread_join()     */
#include <stdlib.h>     /*  malloc(),free(), rand(), srand(), qsort()       */
#include <string.h>     /*  memcpy(), strlen()                              */
#include <unistd.h>     /*  read(), close(), fstat()                        */
#include <sys/types.h>  /*  open(), fstat()                                 */
#include <sys/stat.h>   /*  open(), fstat()                                 */
#include <fcntl.h>      /*  open(), For O_*                                 */
#include <time.h>       /*  time()                                          */
#include <assert.h>     /*  assert()                                        */
#include <errno.h>      /*  errno for perror()                              */

#define NUM_OF_THREADS              (4)
#define NUM_OF_PTR_ARR_COPIES       (4)
#define CHAR_MAX_NUM                (256)
#define SIZE_OF_CHAR                (1)

enum MT_CNT_SORT_STATUS
{
    MT_SHUFFLE_SORT_SUCCESS,
    FILE_OPERATION_ERROR,
    COPY_DICT_TO_BUFFER_ERROR,
    CREATE_AND_COPY_TO_PTR_ARR_ERROR
};

typedef struct thread_args
{
    char **split_arr;
    size_t split_arr_size;
}thread_args_t;

static void *PartOfShuffledArrSort(void *ptr_args);
static int InitDictBuffer(char **dict_buffer);
static int OpenDictAndFindSize(size_t *dict_size);
static char *MakeBufferAndCopyDict(int dict_fd, size_t dict_size);
static size_t CountWordsInDictBuffer(char *dict_buffer);
static char **MakePtrArrToWordsInBuf(char *dict_buffer, size_t num_of_words);
static void SufflePtrArr(char **words_ptr_arr, size_t arr_size);
static int CMPForArrShuffle(const void *ignore_1, const void *ignore_2);
static int CMPForArrReOrder(const void *ptr1, const void *ptr2);
static void ChosenSortAlgorithm(char **words_ptr_arr, size_t arr_size);
static void SplitToThreadsAndThanJoin(char **words_ptr_array, size_t arr_size);
static size_t FindSplitPartSize(char **split_memory_buffer, 
                                char **memory_buffer_end, 
                                size_t wanted_split_memory_size);

static void BubbleSort(char **arr, size_t arr_size);
static void SwapCharPointers(char **ptr1, char **ptr2);
static void RadixSort(char **arr, size_t arr_size);
static size_t FindMaxLength(char **arr, size_t arr_size);
static void RadixHelper(char **input, char **output, size_t arr_size, 
                        size_t letter);


int main()
{
    char *dict_buffer = NULL;
    char **words_ptr_array = NULL;
    size_t num_of_words = 0;
    size_t arr_size = 0;
    size_t i = 0;
    int status = 0;

    status = InitDictBuffer(&dict_buffer);
    if(MT_SHUFFLE_SORT_SUCCESS != status)
    {
        return status;
    }

    num_of_words = CountWordsInDictBuffer(dict_buffer);
    words_ptr_array = MakePtrArrToWordsInBuf(dict_buffer, num_of_words);
    if(NULL == words_ptr_array)
    {
        return CREATE_AND_COPY_TO_PTR_ARR_ERROR;
    }

    arr_size = num_of_words * NUM_OF_PTR_ARR_COPIES;
    SufflePtrArr(words_ptr_array, arr_size);
    SplitToThreadsAndThanJoin(words_ptr_array, arr_size);
    ChosenSortAlgorithm(words_ptr_array, arr_size);

    for(i = 0; 20 > i; ++i)
    {
        printf("%s\n", words_ptr_array[i]);
    }

    free(dict_buffer);
    free(words_ptr_array);   
    
    return MT_SHUFFLE_SORT_SUCCESS;
}

static void *PartOfShuffledArrSort(void *ptr_args)
{
    thread_args_t *args = {0};

    assert(NULL != ptr_args);

    args = (thread_args_t *)ptr_args;
    ChosenSortAlgorithm(args->split_arr, args->split_arr_size);

    return NULL;
}

static int InitDictBuffer(char **dict_buffer)
{
    size_t dict_size = 0;
    int dict_fd = 0;
    
    dict_fd = OpenDictAndFindSize(&dict_size);
    if (-1 == dict_fd)
    {
        return FILE_OPERATION_ERROR;
    }
    
    *dict_buffer = MakeBufferAndCopyDict(dict_fd, dict_size);
    if(NULL == *dict_buffer)
    {
        return COPY_DICT_TO_BUFFER_ERROR;
    }

    return MT_SHUFFLE_SORT_SUCCESS;
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

static char *MakeBufferAndCopyDict(int dict_fd, size_t dict_size)
{
    char *memory_buffer = NULL;
    ssize_t status = 0;

    memory_buffer = (char *)malloc(dict_size + 1);
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
    *(memory_buffer + dict_size) = '\0';

    close(dict_fd);

    return memory_buffer;
}

static size_t CountWordsInDictBuffer(char *dict_buffer)
{
    char *ptr_buffer = NULL;
    long words_counter = 0;

    assert(NULL != dict_buffer);

    ptr_buffer = dict_buffer;
    while('\0' != *ptr_buffer)
    {
        if('\n' == *ptr_buffer && '\n' == *(ptr_buffer + 1))
        {
            *ptr_buffer = '\0';
            ++ptr_buffer;
        }
        if('\n' == *ptr_buffer)
        {
            *ptr_buffer = '\0';
            ++words_counter;
        }
        ++ptr_buffer;
    }

    return words_counter;
}

static char **MakePtrArrToWordsInBuf(char *dict_buffer, size_t num_of_words)
{
    char **words_ptr_arr = NULL;
    char **ptr_arr = NULL;
    char *ptr_buffer = NULL;
    size_t i = 0;

    assert(NULL != dict_buffer);

    words_ptr_arr = (char **)malloc(sizeof(char *) * num_of_words * 
                                        NUM_OF_PTR_ARR_COPIES);
    if(NULL == words_ptr_arr)
    {
        perror("malloc failed");
        free(dict_buffer);

        return NULL;
    }

    ptr_buffer = dict_buffer;
    ptr_arr = words_ptr_arr;
    for(i = 0; num_of_words > i; ++i)
    {
        *ptr_arr = ptr_buffer;
        ++ptr_arr;
        while('\0' != *ptr_buffer++);
    }
    
    for(i = 1; NUM_OF_PTR_ARR_COPIES > i; ++i)
    {
        memcpy(words_ptr_arr + (i * num_of_words), 
                words_ptr_arr, sizeof(char *) * num_of_words);
    }

    return words_ptr_arr;
}

static void SufflePtrArr(char **words_ptr_arr, size_t arr_size)
{
    assert(NULL != words_ptr_arr);

    srand(time(NULL));
    qsort((void *)words_ptr_arr, arr_size, sizeof(char *), CMPForArrShuffle);
}

static int CMPForArrShuffle(const void *ignore_1, const void *ignore_2)
{
    (void)ignore_1;
    (void)ignore_2;

    return (rand() % 1000) - 500;
}

static int CMPForArrReOrder(const void *ptr1, const void *ptr2)
{
    const unsigned char *word1 = NULL;
    const unsigned char *word2 = NULL;
    
    assert(NULL != ptr1);
    assert(NULL != ptr2);

    word1 = *(const unsigned char **)ptr1;
    word2 = *(const unsigned char **)ptr2;

    while('\0' != *word1 && *word1 == *word2)
	{
		++word1;
		++word2;
	}

    return *word1 - *word2;
}

static void SplitToThreadsAndThanJoin(char **words_ptr_array, size_t arr_size)
{
    thread_args_t thread_args[NUM_OF_THREADS] = {0};
    pthread_t thread_id[NUM_OF_THREADS] = {0};
    size_t split_arr_size;
    size_t i = 0;
    
    assert(NULL != words_ptr_array);
    
    split_arr_size = (arr_size / NUM_OF_THREADS);
    for(i = 0; NUM_OF_THREADS > i; ++i)
    {
        thread_args[i].split_arr = words_ptr_array + i * split_arr_size;
        thread_args[i].split_arr_size = 
                FindSplitPartSize(thread_args[i].split_arr,
                                    words_ptr_array + arr_size,
                                    split_arr_size);
        pthread_create(&thread_id[i], NULL, PartOfShuffledArrSort, 
                        (void *)&thread_args[i]);
    }
    
    for(i = 0; NUM_OF_THREADS > i; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
}

static size_t FindSplitPartSize(char **split_arr, 
                                char **words_ptr_array_end, 
                                size_t wanted_split_arr_size)
{
    assert(NULL != split_arr);
    assert(NULL != words_ptr_array_end);
    
    return (((size_t)(words_ptr_array_end - split_arr) < 
                wanted_split_arr_size)                      ?
            (size_t)(words_ptr_array_end - split_arr)       :
            wanted_split_arr_size);
}

static void ChosenSortAlgorithm(char **words_ptr_arr, size_t arr_size)
{
    assert(NULL != words_ptr_arr);

    /*RadixSort(words_ptr_arr, arr_size);*/
/*    BubbleSort(words_ptr_arr, arr_size);*/
    qsort((void *)words_ptr_arr, arr_size, sizeof(char *), CMPForArrReOrder);
}

static void BubbleSort(char **arr, size_t arr_size)
{
	size_t i = 0;
	size_t j = 0;
	int stop_flag = 1;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	for(i = 0; i < arr_size && 1 == stop_flag; ++i)
	{
		stop_flag = 0;
		for(j = 0; j < (arr_size - i - 1); ++j)
		{
			if(0 < CMPForArrReOrder((const void *)&arr[j], 
                                    (const void *)&arr[j + 1]))
			{
				SwapCharPointers(&arr[j], &arr[j + 1]);
				stop_flag = 1;
			}
		}
	}
}

static void SwapCharPointers(char **ptr1, char **ptr2)
{
    char *temp = NULL;

    assert(NULL != ptr1);
    assert(NULL != ptr2);

    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

static void RadixSort(char **arr, size_t arr_size)
{
	char **input = NULL;
	char **output = NULL;
	size_t max_letters = 0;
	size_t letter = 1;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	output = (char **)malloc(sizeof(char *) * arr_size);
	if(NULL == output)
	{
		return;
	}
    input = (char **)malloc(sizeof(char *) * arr_size);
    if(NULL == input)
	{
		free(output);
        
        return;
	}
    memcpy(input, arr, sizeof(char *) * arr_size);
	memset(output, 0, sizeof(char *) * arr_size);
    max_letters = FindMaxLength(arr, arr_size);
	for(letter = 0; max_letters > letter; ++letter)
	{
        RadixHelper(input, output, arr_size, max_letters - letter - 1);
	}
	memcpy(arr, input, arr_size * sizeof(char *));
	free(input);
    free(output);
}

static size_t FindMaxLength(char **arr, size_t arr_size)
{
    size_t i = 0;
    size_t max_len = 0;

    for(i = 0; arr_size > i; ++i)
    {
        size_t temp_len = strlen(arr[i]);

        if(temp_len > max_len)
        {
            max_len = temp_len;
        }
    }

    return max_len;
}

static void RadixHelper(char **input, char **output, size_t arr_size, 
                        size_t letter)
{
	size_t i = 0;
	size_t counter[CHAR_MAX_NUM] = {0};
	size_t index = 0;
    size_t temp_len = 0;
	
	assert(NULL != input);
	assert(NULL != output);
	assert(0 != arr_size);

	for(i = 0; i < arr_size; ++i)
	{
        temp_len = strlen(input[i]);

        if(letter > temp_len)
        {
            index = 0;
        }
        else
        {
            index = *(unsigned char *)(input[i] + letter);
        }
		counter[index] += 1;
	}
	for(i = 1; i < CHAR_MAX_NUM; ++i)
	{
		counter[i] += counter[i - 1];
	}
	for(i = 1; i <= arr_size; ++i)
	{
		temp_len = strlen(input[arr_size - i]);
        if(letter > temp_len)
        {
            index = 0;
        }
        else
        {
            index = *(unsigned char *)(input[arr_size - i] + letter);
        }
        
		output[counter[index] - 1] = input[arr_size - i];
		counter[index] -= 1;
	}
    
    memcpy(input, output, sizeof(char *) * arr_size);
}