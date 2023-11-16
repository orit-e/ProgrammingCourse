/************************************************
 *												*
 *		filename:			priority_queue.c	*
 *		written by:     	Orit				*
 *		reviewed by:    	Moris				*
 *		last edit:      	11.08.2020			*
 *												*
 ************************************************/
#include <stdlib.h>	/* malloc(), free(), size_t */
#include <string.h>	/* memset() 				*/
#include <assert.h>	/* assert() 				*/

#include "priority_queue.h"
#include "heap.h"

#define EMPTY 1

struct priority_queue
{
	heap_ty *heap;
};

priority_q_t *PQCreate(pq_cmp_func_t cmp, void *param)
{
	priority_q_t *new_priority_q = NULL;
	
	assert(NULL != cmp);
	
	new_priority_q = (priority_q_t *)malloc(sizeof(priority_q_t));
	if(NULL == new_priority_q)
	{
		return NULL;
	}
	memset(new_priority_q, 0, sizeof(priority_q_t));
	new_priority_q->heap = HeapCreate((heap_cmp_func_ty)cmp, param);
	if(NULL == new_priority_q->heap)
	{
		free(new_priority_q);
		return NULL;
	}
	
	return new_priority_q;
}

void PQDestroy(priority_q_t *priority_queue)
{
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	
	HeapDestroy(priority_queue->heap);
	memset(priority_queue, 0, sizeof(priority_q_t));
	free(priority_queue);
}

void *PQDeQueue(priority_q_t *priority_queue)
{
	void *data = NULL;
	
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	
	data = PQPeek((const priority_q_t *)priority_queue);
	HeapPop(priority_queue->heap);

	return data;
}

int PQEnQueue(priority_q_t *priority_queue, const void *data)
{
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);

	return HeapPush(priority_queue->heap, (void *)data);
}	

void *PQPeek(const priority_q_t *priority_queue)
{
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	
	return HeapPeek(priority_queue->heap);
}

size_t PQSize(const priority_q_t *priority_queue)
{
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	
	return HeapSize(priority_queue->heap);
}

int PQueueIsEmpty(const priority_q_t *priority_queue)
{
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	
	return HeapIsEmpty(priority_queue->heap);
}

void PQClear(priority_q_t *priority_queue)
{
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	
	while(EMPTY != PQueueIsEmpty(priority_queue))
	{
		PQDeQueue(priority_queue);
	}
}

void *PQErase(priority_q_t *priority_queue, 
					pq_match_func_t is_match, 
					const void *param)
{	
	assert(NULL != priority_queue);
	assert(NULL != priority_queue->heap);
	assert(NULL != is_match);
	
	return HeapRemove(priority_queue->heap, (heap_match_func_ty)is_match, 
						(void *)param);
}

