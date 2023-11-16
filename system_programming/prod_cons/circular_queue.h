#ifndef __CIRCULAR_QUEUE_H__
#define __CIRCULAR_QUEUE_H__

#include <stddef.h> /* size_t */

typedef struct circ_q circ_q_t;


circ_q_t *CircQCreate(size_t capacity);

void CircQDestroy(circ_q_t *circ_q);

void CircQEnQueue(circ_q_t *circ_q, int num);

int CircQDeQueue(circ_q_t *circ_q);

size_t CircQIsEmpty(circ_q_t *circ_q);

size_t CircQSize(circ_q_t *circ_q);

#endif /* __CIRCULAR_QUEUE_H__ */