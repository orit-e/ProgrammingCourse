#include <stdio.h>      /*  printf(), scanf()   */
#include <sys/mman.h>   /*  mmap(), munmap()    */
#include <unistd.h>     /*  fstat(), close()    */
#include <sys/types.h>  /*  open(), fstat()     */
#include <sys/stat.h>   /*  open(), fstat()     */
#include <fcntl.h>      /*  open()              */
#include <ctype.h>      /*  tolower()           */
#include <assert.h>     /*  assert()            */

#include "hash.h"
#include "spell_checker.h"

#define HASH_RANGE 19991

static void LoadDictionaryToHashTableIMP(void *dictionary, hash_ty *hash, 
                                            size_t dictionary_size);
static void *GetNextWordIMP(void *dict_location);
static unsigned char StrLenIMP(void *hashed_word);
static size_t HashFunctionIMP(const void *str);
static int StrCmpIMP(const void *data1, const void *data2);

spell_checker_errors_t SpellChecker()
{
    void *map_addr = NULL;
    int dict_fd = 0;
    size_t file_size = 0;
    struct stat dict_stat = {0};
    hash_ty *hash = NULL;
    char buffer[80] = {0};
    char exit[] = "q";

    dict_fd = open("/usr/share/dict/words", O_RDONLY);
    if (OPEN_ERROR == dict_fd)
    {
        return OPEN_ERROR;
    }

    if (FSTAT_ERROR == fstat(dict_fd, &dict_stat))
    {
        close(dict_fd);
        return FSTAT_ERROR;
    }

    file_size = dict_stat.st_size;
    map_addr = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, dict_fd, 0);
    if (MAP_FAILED == map_addr)
    {
        close(dict_fd);
        return MMAP_ERROR;
    }

    hash = HashCreate(HashFunctionIMP, HASH_RANGE, StrCmpIMP);
    if (NULL == hash)
    {
        munmap(map_addr, file_size);
        close(dict_fd);
        return HASH_ERROR;
    }
    LoadDictionaryToHashTableIMP(map_addr, hash, file_size);

    printf("Please type words to spellcheck (to exit press q):\n");
    scanf("%s", buffer);
    while(0 != StrCmpIMP(&exit, &buffer))
    {
        if(NULL != HashFind(hash, &buffer))
        {
            printf("Great spelling!\n");
        }
        else
        {
            printf("Not a word!\n");
        }
        scanf("%s", buffer);
    }
    
    HashDestroy(hash);
    munmap(map_addr, file_size);
    close(dict_fd);

    return SPELL_CHECKER_SUCCESS;
}

static void LoadDictionaryToHashTableIMP(void *dictionary, hash_ty *hash, 
                                            size_t dictionary_size)
{
    void *current_word = dictionary;
    void *end_of_dict = (void *)((char *)dictionary + dictionary_size);

    assert(NULL != dictionary);

    while (end_of_dict > current_word)
    {
        HashInsert(hash, current_word);
        current_word = GetNextWordIMP(current_word);
    }
}

static unsigned char StrLenIMP(void *hashed_word)
{
    char *word = (char *)hashed_word;

    while ('\n' != *word)
    {
        ++word;
    }

    return word - (char *)hashed_word;
}

static void *GetNextWordIMP(void *dict_location)
{
    assert(NULL != dict_location);

    return (void *)((char *)dict_location + StrLenIMP(dict_location) + 1);
}

static size_t HashFunctionIMP(const void *dict_entry)
{
    unsigned long hash = 5381;
    unsigned char *word = (unsigned char *)dict_entry;
    int c = 0;

    while ('\n' != (c = tolower(*word)) && '\0' != c)
    {
        hash = ((hash << 5) + hash) + c; 
        ++word;
    }

    return hash % HASH_RANGE;
}

static int StrCmpIMP(const void *data1, const void *data2)
{
    char *word1 = (char *)data1;
    char *word2 = (char *)data2;
    
    while ('\n' != *word1 && '\0' != *word2 && 
           tolower(*word1) == tolower(*word2))
    {
        ++word1; 
        ++word2;
    }

    return ('\n' == *word1 && '\0' == *word2) ? 0 : *word1 - *word2;
}
