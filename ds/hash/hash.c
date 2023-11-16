/******************************************************************************
 											
				File name:			hash.c			
				Written by:     	Orit			
				Reviewed by:    	Elia
 											
 ******************************************************************************/
#include <assert.h>		/* assert()								*/
#include <string.h>		/* memset()								*/
#include <stdlib.h>		/* malloc(), free()						*/
#include <math.h>       /* pow(), sqrt()                        */

#include "hash.h"
#include "dlist.h"

/******************************************************************************/
/*					structs and typedef declarations:				  */
struct hash
{
    hash_func_ty hash_func;
    hash_cmp_func_ty compare;
    size_t hash_range;
    dlist_t **hash_table;
};

typedef struct hash_match
{
    const hash_ty *hash;
    const void *data_to_find;
}hash_match_t;

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
static hash_ty *InitHashDListsIMP(hash_ty *new_hash, size_t hash_range);
static void IterativeDListDestroyIMP(hash_ty *hash, size_t num_of_dlists);
static int IsMatchIMP(const void *data, const void *hash_find_struct);
static size_t GoToHashFuncIMP(hash_ty *hash, const void *data);
static size_t GetHashRangeIMP(hash_ty *hash);
static dlist_t *GetDListIMP(hash_ty *hash, size_t hash_table_index);
static void FreeHashIMP(hash_ty *hash);

/******************************************************************************/
/*								Hash functions:								  */
hash_ty *HashCreate(hash_func_ty hash_func, size_t hash_range, 
                                          hash_cmp_func_ty compare)
{
    hash_ty *new_hash = NULL;

    assert(NULL != hash_func);
    assert(0 < hash_range);
    assert(NULL != compare);

    new_hash = (hash_ty *) malloc(sizeof(hash_ty) + 
                                    sizeof(dlist_t *) * hash_range);
    if(NULL == new_hash)
    {
        return NULL;
    }
    memset(new_hash, 0, sizeof(hash_ty) + sizeof(dlist_t *) * hash_range);
    new_hash->hash_func = hash_func;
    new_hash->compare = compare;
    new_hash->hash_range = hash_range;
    new_hash->hash_table = (dlist_t **)(new_hash + 1);
    new_hash = InitHashDListsIMP(new_hash, hash_range);
    
    return new_hash;
}

void HashDestroy(hash_ty *hash)
{
    assert(NULL != hash);

    IterativeDListDestroyIMP(hash, GetHashRangeIMP(hash));
    FreeHashIMP(hash);
}

int HashInsert(hash_ty *hash, void *data)
{
    dlist_t *dlist = NULL;
    size_t index = 0;

    assert(NULL != hash);
    assert(NULL != data);

    index = GoToHashFuncIMP(hash, (const void *)data);
    dlist = GetDListIMP(hash, index);

    return (DListEnd(dlist) == DListPushBack(dlist, (const void *)data));
}

void HashRemove(hash_ty *hash, const void *data)
{
    hash_match_t hash_match = {NULL};
    dlist_t *dlist = NULL;
    iterator_t iter = NULL;
    iterator_t iter_end = NULL;
    size_t index = 0;
    

    assert(NULL != hash);
    assert(NULL != data);

    hash_match.hash = hash;
    hash_match.data_to_find = data;
    index = GoToHashFuncIMP(hash, data);
    dlist = GetDListIMP(hash, index);
    iter_end = DListEnd(dlist);
    iter = DListFind(DListBegin(dlist), iter_end, IsMatchIMP, &hash_match);
    if(iter_end != iter)
    {
        DListRemove(iter);
    }
}

size_t HashSize(const hash_ty *hash)
{
    size_t counter = 0;
    size_t i = 0;
    size_t hash_range = 0;

    assert(NULL != hash);

    hash_range = GetHashRangeIMP((hash_ty *)hash);
    for(i = 0; hash_range > i; ++i)
    {
        counter += DListCount(GetDListIMP((hash_ty *)hash, i));
    }
    
    return counter;
}

int HashIsEmpty(const hash_ty *hash)
{
    size_t i = 0;
    size_t hash_range = 0;

    assert(NULL != hash);

    hash_range = GetHashRangeIMP((hash_ty *)hash);
    for(i = 0; hash_range > i && 
        1 == DListIsEmpty(GetDListIMP((hash_ty *)hash, i)); ++i)
    {
        ;
    }
    
    return (i == hash_range);
}

void *HashFind(const hash_ty *hash, const void *data_to_find)
{
    hash_match_t hash_match = {NULL};
    dlist_t *dlist = NULL;
    iterator_t iter = NULL;
    iterator_t iter_end = NULL;
    void *data = NULL;
    size_t index = 0;

    assert(NULL != hash);
    assert(NULL != data_to_find);

    hash_match.hash = hash;
    hash_match.data_to_find = data_to_find;
    index = GoToHashFuncIMP((hash_ty *)hash, data_to_find);
    dlist = GetDListIMP((hash_ty *)hash, index);
    iter_end = DListEnd(dlist);
    iter = DListFind(DListBegin(dlist), iter_end, IsMatchIMP, &hash_match);
    if(iter_end != iter)
    {
        data = DListGetData((const iterator_t)iter);
        if(iter_end != DListPushFront(dlist, data))
        {
            DListRemove(iter);
        }
    }

    return data;
}

int HashForEach(hash_ty *hash, hash_operation_func_ty operation, void *op_param)
{
    size_t i = 0;
    size_t hash_range = 0;
    int status = 0;

    assert(NULL != hash);
    assert(NULL != operation);

    hash_range = GetHashRangeIMP(hash);
    for(i = 0; hash_range > i && 0 == status; ++i)
    {
        status = DListForeach(DListBegin(GetDListIMP(hash, i)),
                                DListEnd(GetDListIMP(hash, i)),
                                operation, op_param);
    }
        
    return status;
}

double HashLoad(const hash_ty *hash)
{
    assert(NULL != hash);

    return ((double)HashSize(hash) / 
            (double)GetHashRangeIMP((hash_ty *)hash));
}

double HashStdev(const hash_ty *hash)
{
    double avg = 0;
    double sum = 0;
    size_t i = 0;
    size_t hash_range = 0;

    assert(NULL != hash);

    avg = HashLoad(hash);
    hash_range = GetHashRangeIMP((hash_ty *)hash);
    for(i = 0; hash_range > i; ++i)
    {
        sum += 
            pow(((double)DListCount(GetDListIMP((hash_ty *)hash, i)) - avg), 2);
    }

    return (sqrt(sum / (hash_range - 1)));
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
static hash_ty *InitHashDListsIMP(hash_ty *new_hash, size_t hash_range)
{
    size_t i = 0;

    assert(NULL != new_hash);
    assert(0 < hash_range);
    
    for(i = 0; hash_range > i; ++i)
    {
        new_hash->hash_table[i] = DListCreate();
        if(NULL == new_hash->hash_table[i])
        {
            IterativeDListDestroyIMP(new_hash, i);
            FreeHashIMP(new_hash);
            
            return NULL;
        }
    }
    
    return new_hash;
}

static void IterativeDListDestroyIMP(hash_ty *hash, size_t num_of_dlists)
{
    size_t i = 0;

    assert(NULL != hash);
    
    for(i = 0; num_of_dlists > i; ++i)
    {
        DListDestroy(GetDListIMP(hash, i));
    }  
}

static size_t GoToHashFuncIMP(hash_ty *hash, const void *data)
{
    assert(NULL != hash);

    return hash->hash_func(data);
}

static size_t GetHashRangeIMP(hash_ty *hash)
{
    assert(NULL != hash);

    return hash->hash_range;
}

static dlist_t *GetDListIMP(hash_ty *hash, size_t hash_table_index)
{
    assert(NULL != hash);
    assert(GetHashRangeIMP(hash) > hash_table_index);

    return hash->hash_table[hash_table_index];
}

static int IsMatchIMP(const void *data, const void *hash_find_struct)
{
    hash_match_t *hash_match = (hash_match_t *)hash_find_struct;

    return (0 == hash_match->hash->compare(data, hash_match->data_to_find));
}

static void FreeHashIMP(hash_ty *hash)
{
    assert(NULL != hash);

    memset(hash, 0, sizeof(hash_ty) + 
                    sizeof(dlist_t *) * GetHashRangeIMP(hash));
    free(hash);
}
