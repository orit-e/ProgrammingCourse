/********************************************
 *											*
 *		filename:			sorted_list.c	*
 *		written by:     	Orit			*
 *		reviewed by:    	Moris			*
 *		last edit:      	10.08.2020		*
 *											*
 ********************************************/
#include <stdlib.h>	/* malloc, free, size_t */
#include <assert.h>	/* assert */
#include <string.h>	/* memset */
#include "sorted_list.h"

#define EQUAL 0

struct sorted_list
{
	dlist_t *dlist;
	sorted_list_cmp_func_t cmp;
};

sorted_list_t *SortedListCreate(sorted_list_cmp_func_t cmp)
{
	sorted_list_t *new_sorted = (sorted_list_t *)malloc(sizeof(sorted_list_t));
	
	assert(NULL != cmp);
	
	if(NULL == new_sorted)
	{
		return NULL;
	}
	memset(new_sorted, 0, sizeof(sorted_list_t));
	new_sorted->dlist = DListCreate();
	if(NULL == new_sorted->dlist)
	{
		free(new_sorted);
		return NULL;
	}
	new_sorted->cmp = cmp;
	
	return new_sorted;
}

void SortedListDestroy(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	DListDestroy(sorted_list->dlist);
	memset(sorted_list, 0, sizeof(sorted_list_t));
	free(sorted_list);
	sorted_list = NULL;
}

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	return DListIsEmpty(sorted_list->dlist);
}

void *SortedListGetData(const iterator_t iter)
{
	assert(NULL != iter);
	
	return DListGetData(iter);
}

static iterator_t SortedListCMP(iterator_t from, 
								iterator_t to, 
								sorted_list_cmp_func_t cmp,
								const void *data)
{
	iterator_t iter = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != cmp);
	
	for(iter = from; 
		iter != to && EQUAL >= cmp(SortedListGetData(iter), data); 
		iter = SortedListNext(iter))
	{
		;
	}
	
	return iter;
}	

iterator_t SortedListInsert(sorted_list_t *sorted_list, const void *data)
{
	iterator_t place_to_insert = NULL;
		
	assert(NULL != sorted_list);
	
	place_to_insert = SortedListCMP(SortedListBegin(sorted_list), 
												SortedListEnd(sorted_list),
												sorted_list->cmp, data);

	
	return DListInsertBefore(place_to_insert, data);
}

iterator_t SortedListRemove(iterator_t iter)
{
	assert(NULL != iter);
	
	return DListRemove(iter);
}

size_t SortedListCount(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	return DListCount(sorted_list->dlist);
}

void *SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	return DListPopFront(sorted_list->dlist);
}

void *SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	return DListPopBack(sorted_list->dlist);
}

iterator_t SortedListFindIf(iterator_t from,
							iterator_t to,
							sorted_list_match_func_t is_match,
							const void *param)
{
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);
	
	return DListFind(from, to, is_match, param);
}

iterator_t SortedListFind(iterator_t from,
						  iterator_t to,
						  const void *data,
						  sorted_list_t *sorted_list)
{
	iterator_t iter = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	
	for(iter = from; 
		 iter != to && EQUAL > sorted_list->cmp(SortedListGetData(iter), data); 
		iter = SortedListNext(iter))
	{
		;
	}
	if(iter != to && EQUAL != sorted_list->cmp(SortedListGetData(iter), data))
	{
		iter = SortedListEnd(sorted_list);
	}
	
	return iter;
}

int SortedListForeach(iterator_t from,
					  iterator_t to,
					  sorted_list_operation_func_t is_operated,
					  const void *param)
{
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_operated);
	
	return DListForeach(from, to, (operation_func_t)is_operated, param);
}

void SortedListMerge(sorted_list_t *dst, sorted_list_t *src)
{
	iterator_t iter_where = NULL;
	iterator_t iter_from = NULL;
	iterator_t iter_to = NULL;
	
	assert(NULL != dst);
	assert(NULL != src);
	
	iter_where = SortedListBegin(dst);
	iter_from = SortedListBegin(src);
	
	while(SortedListEnd(src) != iter_from)
	{
		iter_where = SortedListCMP(iter_where, SortedListEnd(dst), dst->cmp, 
									SortedListGetData(iter_from));
		if(SortedListEnd(dst) != iter_where)
		{
			iter_to = SortedListCMP(iter_from, SortedListEnd(src), dst->cmp, 
									SortedListGetData(iter_where));
			iter_where = DListSplice(iter_where, iter_from, iter_to);
			iter_from = iter_to;
		}
		else
		{
			DListSplice(iter_where, iter_from, SortedListEnd(src));
		}
	}
	
}

/***********Iterators Functions**************/
iterator_t SortedListEnd(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	return DListEnd(sorted_list->dlist);
}

iterator_t SortedListBegin(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	return DListBegin(sorted_list->dlist);
}

iterator_t SortedListNext(const iterator_t current)
{
	assert(NULL != current);
	
	return DListNext(current);
}

iterator_t SortedListPrev(const iterator_t current)
{
	assert(NULL != current);
	
	return DListPrev(current);
}

int SortedListIsEqual(const iterator_t iter1, const iterator_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return DListIsEqual(iter1, iter2);
}

