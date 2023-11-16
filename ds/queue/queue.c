#include <stdlib.h>	/* malloc(), free() */
#include <assert.h>	/* assert() 		*/
#include <string.h>	/* memset() 		*/

#include "queue.h"
#include "../slist/slist.h"

struct queue
{
	slist_t *slist;	
};

queue_t *QCreate()
{
	queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
	
	if(NULL == new_queue)
	{
		return NULL;
	}
	new_queue->slist = SListCreate();
	if(NULL == new_queue->slist)
	{
		memset(new_queue, 0, sizeof(queue_t));
		free(new_queue);
		new_queue = NULL;
	}
	
	return new_queue;
}

void QDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SListDestroy(queue->slist);
	memset(queue, 0, sizeof(queue_t));
	free(queue);
	queue = NULL;
}

void QDeQueue(queue_t *queue)
{
	assert(NULL != queue);
	
	SListRemove(SListBegin(queue->slist));
}

int QEnQueue(queue_t *queue, const void *data)
{
	iterator_t temp = SListInsert(SListEnd(queue->slist), data);
	
	assert(NULL != queue);
	
	return (SListEnd(queue->slist) == temp);
}

void *QPeek(const queue_t *queue)
{
	assert(NULL != queue);
	
	return SListGetData(SListBegin(queue->slist));
}

size_t QSize(const queue_t *queue)
{
	assert(NULL != queue);
	
	return SListCount(queue->slist);
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return SListIsEmptyList(queue->slist);
}

void QAppend(queue_t *dst, queue_t *src)
{
	assert(NULL != src);
	assert(NULL != dst);
	
	SListAppend(dst->slist, src->slist);
}
