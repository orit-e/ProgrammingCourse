#ifndef __QUEUE__H__
#define __QUEUE__H__		   

#include <stddef.h>

typedef struct queue queue_t;

/* DESCRIPTION
* creates the queue
*
* @param: 
*
*				
* @return: 	pointer to the new queue	
*/
queue_t *QCreate();


/* DESCRIPTION
* destroys the queue and frees allocated memory
*
* @param: 
* *queue:	pointer to the queue
*				
* @return: 	no return value
*/
void QDestroy(queue_t *queue);


/* DESCRIPTION
* removes the first element from the queue
*
* @param: 
* *queue:	pointer to the queue 
*				
* @return: 	no return
*/
void QDeQueue(queue_t *queue); 


/* DESCRIPTION
* adds new element to the end of the queue
*
* @param: 
* *queue:	pointer to the queue 
* *data:	pointer to element we want to add
*				
* @return: 	0 for success, 1 for failure
*/
int QEnQueue(queue_t *queue, const void *data);


/* DESCRIPTION
* returns the data of the first element in the queue 
*
* @param: 
* *queue:	pointer to the queue
* 
*				
* @return: 	pointer to the first data in the queue
*/
void *QPeek(const queue_t *queue);


/* DESCRIPTION
* returns the current size of the queue
*
* @param: 
* *queue:	pointer to the queue
*				
* @return: 	number of elements stored in the queue
*/
size_t QSize(const queue_t *queue);


/* DESCRIPTION
* Checks if there are no elements in the queue
*
* @param: 
* *queue:	pointer to the queue 
*				
* @return: 	boolean value 1 empty, 0 not empty.
*/
int QueueIsEmpty(const queue_t *queue);


/* DESCRIPTION
* appends the src queue to the dst queue,
* src queue is left empty
*
* @param: 
* *dst:	pointer to the destination queue, 
* *src:	pointer to the source queue 
*				
* @return: 	no return
*/
void QAppend(queue_t *dst, queue_t *src);


#endif /* __QUEUE_H__ */
