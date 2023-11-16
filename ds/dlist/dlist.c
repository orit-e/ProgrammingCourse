/********************************************
 *											*
 *		filename:			dlist.c			*
 *		written by:     	Orit			*
 *		reviewed by:    	Ran				*
 *		last edit:      	08.08.2020		*
 *											*
 ********************************************/

#include <stdlib.h>	/* malloc, free, size_t */
#include <assert.h>	/* assert */
#include <string.h>	/* memset */

#include "dlist.h"

/******************************************************************************/
/*					define, enum and structs declarations:				  	  */
#define HEAD_AND_TAIL 2

enum IS_SUCCESS_MATCH 
{
	FAIL_IS_MATCH = 0,
	SUCCESS_IS_MATCH
};

enum IS_SUCCESS_FOR_EACH 
{
	SUCCESS_FOR_EACH = 0,
	FAIL_FOR_EACH
};

struct dlist 
{
	node_t *head;
	node_t *tail;	
};

struct node
{
	const void *data;
	node_t *next;
	node_t *prev;
};

struct struct_for_find
{
	match_func_t is_match;
	iterator_t found;
	const void* param;
};

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
static iterator_t LoopUntilTail(iterator_t iter);
static void ConnectNodes(iterator_t iter_prev, iterator_t iter_next);
static int FindForEach(void *data, void *struct_param_for_find);

/******************************************************************************/
/*								DList functions:							  */
dlist_t *DListCreate()
{
	dlist_t *new_dlist = (dlist_t *)malloc(sizeof(dlist_t) + 
											sizeof(node_t) * HEAD_AND_TAIL);
	
	if(NULL == new_dlist)
	{
		return NULL;
	}
	memset(new_dlist, 0, sizeof(dlist_t) + sizeof(node_t) * HEAD_AND_TAIL);

	new_dlist->head = (iterator_t)(new_dlist + 1);
	new_dlist->tail = (iterator_t)((char *)(new_dlist + 1) + sizeof(node_t));
	ConnectNodes(new_dlist->head, new_dlist->tail);
	
	return new_dlist;
}

void DListDestroy(dlist_t *dlist)
{
	iterator_t iter = NULL;
	
	assert(NULL != dlist);
	
	iter = DListBegin(dlist);
	for(iter = DListBegin(dlist); 
		iter != DListEnd(dlist);
		iter = DListRemove(iter));
	memset(dlist, 0, sizeof(dlist_t) + sizeof(node_t) * HEAD_AND_TAIL);

	free(dlist);

	dlist = NULL;
}

int DListIsEmpty(const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return (DListBegin(dlist) == DListEnd(dlist));
}


void *DListGetData(const iterator_t iter)
{
	assert(NULL != iter);
	
	return (void *)iter->data;
}

void DListSetData(iterator_t iter, const void *data)
{
	assert(NULL != iter);
	
	iter->data = (void *)data;
}

iterator_t DListInsertBefore(iterator_t where, const void *data)
{
	iterator_t inserted_node = NULL;
	iterator_t prev_iter = NULL;
	
	assert(NULL != where);
	
	inserted_node = (iterator_t)malloc(sizeof(node_t));
	if(NULL == inserted_node)
	{
		where = LoopUntilTail(where);
		return where;
	}
	memset(inserted_node, 0, sizeof(node_t));

	inserted_node->data = data;
	prev_iter = where->prev;
	ConnectNodes(inserted_node, where);
	ConnectNodes(prev_iter, inserted_node);
	
	return inserted_node;
}

iterator_t DListInsertAfter(iterator_t where, const void *data)
{	
	assert(NULL != where);

	return DListInsertBefore(DListNext(where), data);
}

iterator_t DListRemove(iterator_t iter)
{
	iterator_t iter_next = NULL;
	iterator_t iter_prev = NULL;
	
	assert(NULL != iter);
	assert(NULL != DListNext(iter));
	
	iter_next = DListNext(iter);
	iter_prev = DListPrev(iter);

	memset(iter, 0, sizeof(node_t));
	free(iter);

	ConnectNodes(iter_prev, iter_next);
	
	return iter_next;
}

size_t DListCount(const dlist_t *dlist)
{
	size_t counter = 0;
	iterator_t iter = NULL;
	iterator_t iter_end = NULL;
	
	assert(NULL != dlist);
	
	iter_end = DListEnd(dlist);
	for(iter = DListBegin(dlist);
		iter_end != iter;
		iter = DListNext(iter))
	{
		++counter;
	}
	
	return counter;
}

iterator_t DListPushFront(dlist_t *dlist, const void *data)
{
	assert(NULL != dlist);
	
	return DListInsertBefore(DListBegin(dlist), data);
}

iterator_t DListPushBack(dlist_t *dlist, const void *data)
{
	assert(NULL != dlist);
	
	return DListInsertBefore(DListEnd(dlist), data);
}

void *DListPopFront(dlist_t *dlist)
{
	iterator_t iter = NULL;
	void *data = NULL;
	
	assert(NULL != dlist);
	
	iter = DListBegin(dlist);
	data = (void *)iter->data;

	DListRemove(iter);
	
	return data;
}

void *DListPopBack(dlist_t *dlist)
{
	iterator_t iter = NULL;
	void *data = NULL;
	
	assert(NULL != dlist);
	
	iter = DListPrev(DListEnd(dlist));
	data = (void *)iter->data;

	DListRemove(iter);
	
	return data;
}

iterator_t DListFind(iterator_t from, iterator_t to, match_func_t is_match, 
						const void *param)
{
	iterator_t iter = NULL;
	struct struct_for_find find_struct = {NULL};
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);
	
	find_struct.is_match = is_match;
	find_struct.found = from;
	find_struct.param = param;

	DListForeach(from, to, &FindForEach, (const void *)&find_struct);

	iter = find_struct.found;
	if(to == iter)
	{
		iter = LoopUntilTail(iter);
	}
	
	return iter;
}

int DListForeach(iterator_t from, iterator_t to, operation_func_t is_operated, 
					const void *param)
{
	iterator_t iter = NULL;
	int is_success = SUCCESS_FOR_EACH;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_operated);
	
	for(iter = from; 
		SUCCESS_FOR_EACH == is_success && to != iter; 
		iter = DListNext(iter))
	{
		is_success = is_operated(DListGetData(iter), (void *)param);
	}
	
	return is_success;
}

iterator_t DListSplice(iterator_t where, iterator_t from, iterator_t to)
{
	iterator_t iter_prev_to = NULL;
	iterator_t iter_prev_from = NULL;
	iterator_t iter_prev_where = NULL;
	
	assert(NULL != where);
	assert(NULL != from);
	assert(NULL != to);
	
	iter_prev_to = DListPrev(to);
	iter_prev_from = DListPrev(from);
	iter_prev_where = DListPrev(where);

	ConnectNodes(iter_prev_from, to);
	ConnectNodes(iter_prev_where, from);
	ConnectNodes(iter_prev_to, where);
	
	return where;
}
	
int DListMultiFind(iterator_t from, iterator_t to, match_func_t is_match, 
					void const *param, dlist_t *output_list)
{
	iterator_t iter = NULL;
	iterator_t iter_push_back = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);
	assert(NULL != output_list);
	
	iter_push_back = from;
	for(iter = from; 
		iter != to && NULL != iter_push_back; 
		iter_push_back = DListPushBack(output_list, iter->data), 
		iter = DListNext(iter))
	{
		iter = DListFind(iter, to, is_match, param);
	}
	
	return(NULL == iter_push_back);
}

/******************************************************************************/
/*							Iterator functions:							  	  */

iterator_t DListEnd(const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return dlist->tail;
}

iterator_t DListBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);
	assert(NULL != dlist->head);
	
	return dlist->head->next;
}

iterator_t DListNext(const iterator_t current)
{
	assert(NULL != current);
	
	return current->next;
}

iterator_t DListPrev(const iterator_t current)
{
	assert(NULL != current);
	
	return current->prev;
}

int DListIsEqual(const iterator_t iter1, const iterator_t iter2)
{	
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}

/******************************************************************************/
/*								Iterators Functions:						  */
static iterator_t LoopUntilTail(iterator_t iter)
{
	while(NULL != DListNext(iter))
	{
		iter = DListNext(iter);
	}
	
	return iter;
}

static void ConnectNodes(iterator_t iter_prev, iterator_t iter_next)
{
	iter_prev->next = iter_next;
	iter_next->prev = iter_prev;
}

static int FindForEach(void *data, void *struct_param_for_find)
{
	struct struct_for_find *temp_find = 
								(struct struct_for_find *)struct_param_for_find;
	int is_found = FAIL_IS_MATCH;
	
	is_found = temp_find->is_match((const void *)data, temp_find->param);
	if(FAIL_IS_MATCH == is_found)
	{
		temp_find->found = DListNext(temp_find->found);
	}
	
	return is_found;
}