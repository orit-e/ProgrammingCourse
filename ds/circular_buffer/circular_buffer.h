#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <unistd.h>
typedef struct circ_buffer circ_buffer_t;


/* DESCRIPTION
* Creates the queue
*
* @param 
* capacity		Buffer length
*				
* @return
* Pointer to struct of type circ_buffer_t
*/
circ_buffer_t *CBufferCreate(size_t capacity);

/* DESCRIPTION
* Frees the buffer
*
* @param 
* buf			pointer to the buffer struct variable
*				
* @return
* None
*/
void CBufferDestroy(circ_buffer_t *buf);

/* DESCRIPTION
* Attemps to read up to count bytes from buffer and stores in dst
*
* @param 
* src			pointer to the buffer struct variable
* dst			pointer to memory to store bytes
* count			number of bytes
*				
* @return
* On success, the number of bytes written is returned.  
* On error, -1 is returned
*/
ssize_t CBufferRead(circ_buffer_t *src, void *dst, size_t count);

/* DESCRIPTION
* Writes up to count bytes from src to the buffer dst
*
* @param 
* src			pointer to memory to read bytes
* dst			pointer to the buffer struct variable
* count			number of bytes
*				
* @return
* On success, the number of bytes written is returned.  
* On error, -1 is returned
*/
ssize_t CBufferWrite(circ_buffer_t *dst, const void *src, size_t count);

/* DESCRIPTION
* Checks if buffer is empty
*
* @param
* buf			pointer to the buffer struct variable
*				
* @return
* 0 if not empty, otherwise 1.
*/
int CBufferIsEmpty(circ_buffer_t *buf);

/* DESCRIPTION
* Returns the number of used bytes in buffer
*
* @param 
* buf			pointer to the buffer struct variable
*				
* @return
* Number of bytes
*/
size_t CBufferSize(circ_buffer_t *buf);

/* DESCRIPTION
* Returns the number of free bytes in buffer
*
* @param 
* buf			pointer to the buffer struct variable
*				
* @return
* Number of free space (bytes).
*/
size_t CBufferFreeSpace(circ_buffer_t *buf);

#endif /*__CIRCULAR_BUFFER_H__*/
