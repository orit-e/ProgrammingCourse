#include <stdlib.h>	/* malloc(), free() */
#include <assert.h>	/* assert() 	    */
#include <string.h>	/* memset() 		*/

#include "circular_queue.h"

struct circ_q
{
    size_t q_capacity;
    size_t q_start;
    size_t q_end;
    int queue[1];
};

circ_q_t *CircQCreate(size_t capacity)
{
    circ_q_t *new_circ_q = NULL;

    assert(0 < capacity);

    new_circ_q = (circ_q_t *)malloc(sizeof(circ_q_t) + 
                                    sizeof(int) * (capacity - 1));
    if(NULL == new_circ_q)
    {
        return NULL;
    }
    memset(new_circ_q, 0, sizeof(circ_q_t) + sizeof(int) * (capacity - 1));
    new_circ_q->q_capacity = capacity;
    
    return new_circ_q;
}

void CircQDestroy(circ_q_t *circ_q)
{
    assert(NULL != circ_q);

    memset(circ_q, 0, sizeof(circ_q_t) + 
                        sizeof(int) * (circ_q->q_capacity - 1));
    free(circ_q);
}

void CircQEnQueue(circ_q_t *circ_q, int num)
{
    assert(NULL != circ_q);

    circ_q->queue[circ_q->q_end] = num;
    circ_q->q_end = (circ_q->q_end + 1) % circ_q->q_capacity;
}

int CircQDeQueue(circ_q_t *circ_q)
{
    int num = 0;

    assert(NULL != circ_q);

    num = circ_q->queue[circ_q->q_start];
    circ_q->q_start = (circ_q->q_start + 1) % circ_q->q_capacity;

    return num;
}

size_t CircQIsEmpty(circ_q_t *circ_q)
{
    assert(NULL != circ_q);

    return (circ_q->q_start == circ_q->q_end);
}

size_t CircQSize(circ_q_t *circ_q)
{
    assert(NULL != circ_q);

    if(circ_q->q_end >= circ_q->q_start)
    {
        return (circ_q->q_end - circ_q->q_start);
    }
    else
    {
        return (circ_q->q_end + (circ_q->q_capacity - circ_q->q_start));
    }
}