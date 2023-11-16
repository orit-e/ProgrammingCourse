/********************************************
 *											*
 *		filename:			slist.c			*
 *		written by:     	Orit			*
 *		reviewed by:    	Yona			*
 *		last edit:      	3.8.2020		*
 *											*
 ********************************************/

#include <stdlib.h>	/* malloc(), realloc(), free(), size_t 	*/
#include <assert.h>	/* assert() 							*/
#include <string.h>	/* memset() 							*/

#include "slist.h"

/******************************************************************************/
/*						enum and structs declarations:						  */
enum IS_SUCCESS 
{
	FAIL = 0, 
	SUCCESS
};

struct slist 
{
	node_t *head;
	node_t *tail;	
};

struct node
{
	void *value;
	node_t *next;
};

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
static iterator_t LoopUntilTail(iterator_t iter);
static void SlistPointToNewTail(iterator_t iter_of_new_tail);

/******************************************************************************/
/*								SList functions:							  */
slist_t *SListCreate()
{
	slist_t *new_slist = (slist_t *)malloc(sizeof(slist_t));
	
	if(NULL == new_slist)
	{
		return NULL;
	}
	memset(new_slist, 0, sizeof(slist_t));

	new_slist->head = (iterator_t)malloc(sizeof(node_t));
	new_slist->tail = (iterator_t)malloc(sizeof(node_t));
	if(NULL == new_slist->head || NULL == new_slist->tail)
	{
		free(new_slist->head);
		free(new_slist->tail);
		free(new_slist);
		return NULL;
	}
	memset(new_slist->head, 0, sizeof(node_t));
	memset(new_slist->tail, 0, sizeof(node_t));
	
	new_slist->head->next = new_slist->tail;
	new_slist->tail->value = new_slist;
	
	return new_slist;
}

void SListDestroy(slist_t *slist)
{
	iterator_t iter = SListBegin(slist);
	
	assert(NULL != slist);

	for(iter = SListBegin(slist); 
		iter != SListEnd(slist);
		iter = SListRemove(iter));
	
	memset(slist->head, 0, sizeof(node_t));
	free(slist->head);

	memset(slist->tail, 0, sizeof(node_t));
	free(slist->tail);

	memset(slist, 0, sizeof(slist_t));
	free(slist);

	slist = NULL;
}

int SListIsEmptyList(const slist_t *slist)
{
	assert(NULL != slist);
	
	return (SListBegin(slist) == SListEnd(slist));
}

void *SListGetData(const iterator_t iter)
{
	assert(NULL != iter);
	assert(NULL != SListNext(iter));
	
	return iter->value;
}

void SListSetData(iterator_t iter, const void *data)
{
	assert(NULL != iter);
	assert(NULL != SListNext(iter));
	
	iter->value = (void *)data;
}

iterator_t SListFindInSlist(iterator_t from, iterator_t to, 
							match_func_t is_match, void *param)
{
	int is_yes = is_match((const void *)SListGetData(from), param);
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);
	
	while(SUCCESS != is_yes && to != SListNext(from))
	{
		from = SListNext(from);
		is_yes = is_match(SListGetData(from), param);
	}
	if(FAIL == is_yes)
	{
		LoopUntilTail(from);
	}
	
	return from;
}

iterator_t SListInsert(iterator_t where, const void *data)
{
	iterator_t inserted_node = NULL;
	
	assert(NULL != where);
	
	inserted_node = (iterator_t)malloc(sizeof(node_t));
	if(NULL == inserted_node)
	{
		where = LoopUntilTail(where);
		return where;
	}
	memset(inserted_node, 0, sizeof(node_t));

	*inserted_node = *where;
	where->value = (void *)data;
	where->next = inserted_node;

	if(NULL == inserted_node->next)
	{
		SlistPointToNewTail(inserted_node);
	}
	
	return where;
}

iterator_t SListRemove(iterator_t iter)
{
	iterator_t to_remove = NULL;
	
	assert(NULL != iter);
	assert(NULL != SListNext(iter));
	
	to_remove = SListNext(iter);
	*iter = *to_remove;
	memset(to_remove, 0, sizeof(node_t));
	free(to_remove);

	if(NULL == SListNext(iter))
	{
		SlistPointToNewTail(iter);
	}
	
	return iter;
}

size_t SListCount(const slist_t *slist)
{
	size_t counter = 0;
	iterator_t iter = NULL;
	
	assert(NULL != slist);
	
	for(iter = SListBegin(slist);
		SListEnd(slist) != iter;
		iter = SListNext(iter))
	{
		++counter;
	}
	
	return counter;
}

int SListForeach(iterator_t from, iterator_t to, operation_func_t is_operated, 
					void *param)
{
	int is_success = SUCCESS;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_operated);
	
	while(to != from && FAIL != is_success)
	{
		is_success = is_operated(SListGetData(from), param);
		from = SListNext(from);
	}
	
	return is_success;
}

/******************************************************************************/
/*								Iterators Functions:						  */
iterator_t SListEnd(const slist_t *slist)
{
	assert(NULL != slist);
	
	return slist->tail;
}

iterator_t SListBegin(const slist_t *slist)
{
	assert(NULL != slist);
	
	return slist->head->next;
}

iterator_t SListNext(const iterator_t current)
{
	assert(NULL != current);
	
	return current->next;
}
	
int SListIsEqual(const iterator_t iter1, const iterator_t iter2)
{	
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}

/******************************************************************************/
/*								For queue exercise:							  */
void SListAppend(slist_t *dst, slist_t *src)
{
	iterator_t iter_src_end = NULL;
	iterator_t iter_src_begin = NULL;
	iterator_t iter_dst = NULL;
	
	assert(NULL != dst);
	assert(NULL != src);

	iter_src_end = SListEnd(src);
	iter_src_begin = SListBegin(src);
	iter_dst = SListEnd(dst);

	*iter_dst = *iter_src_begin;
	iter_src_begin->next = NULL;
	iter_src_end->value = dst;
	src->tail = iter_src_begin;
	dst->tail = iter_src_end;
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
static iterator_t LoopUntilTail(iterator_t iter)
{
	while(NULL != SListNext(iter))
	{
		iter = SListNext(iter);
	}
	
	return iter;
}

static void SlistPointToNewTail(iterator_t iter_of_new_tail)
{
	slist_t *my_slist = iter_of_new_tail->value;
		
	my_slist->tail = iter_of_new_tail;
}