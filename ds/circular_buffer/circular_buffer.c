/****************************************************
 *													*
 *		filename:			circular_buffer.c		*
 *		written by:     	Orit					*
 *		reviewed by:    	Dor						*
 *		last edit:      	06.08.2020				*
 *													*
 ****************************************************/
#include <stdlib.h>				/* malloc, free */
#include <assert.h>				/* assert 		*/
#include <string.h>				/* memset 		*/
#include <errno.h>				/* errno 		*/

#include "circular_buffer.h"

#define OFFSETOF(type_truct, m) \
		((size_t)((char *)&((type_truct *)0)->m - (char *)0))

#define STEP 1
#define ERROR -1
#define EMPTY 0
#define NO_FREE_SPACE 0
#define YES_IS_EMPTY 1

struct circ_buffer
{
	size_t read_offset;
	size_t capacity;
	size_t size;
	char buffer[1];
};

circ_buffer_t *CBufferCreate(size_t capacity)
{
	circ_buffer_t *new_circ_buffer = 
					(circ_buffer_t *)malloc(OFFSETOF(circ_buffer_t, buffer) + 
											sizeof(char) * capacity);
	
	assert(0 != capacity);
	
	if(NULL == new_circ_buffer)
	{
		return NULL;
	}
	memset(new_circ_buffer, 0, OFFSETOF(circ_buffer_t, buffer) + 
								sizeof(char) * capacity);
	new_circ_buffer->capacity = capacity;
	
	return new_circ_buffer;
}

void CBufferDestroy(circ_buffer_t *buf)
{
	assert(NULL != buf);
	
	memset(buf, 0, sizeof(circ_buffer_t) + buf->capacity - 1);
	free(buf);
	buf = NULL;
}

ssize_t CBufferRead(circ_buffer_t *src, void *dst, size_t count)
{
	void *iter_dst = dst;
	size_t read_size = count < src->size ? count : src->size;
	size_t how_much_to_read = 
					(src->read_offset + read_size) < src->capacity ?
					read_size :
					read_size - (src->read_offset + read_size) % src->capacity;
	
	assert(NULL != src);
	assert(NULL != dst);
	assert(0 != count);
	
	if(YES_IS_EMPTY == CBufferIsEmpty(src))
	{
		errno = ENODATA;
		return ERROR;
	}
	memcpy(iter_dst, (void *)&src->buffer[src->read_offset], how_much_to_read);
	iter_dst = (void *)((char *)iter_dst + how_much_to_read);
	how_much_to_read = read_size - how_much_to_read;
	memcpy(iter_dst, (void *)src->buffer, how_much_to_read);
	src->size -= read_size;
	src->read_offset = (src->read_offset + read_size) % src->capacity;
	
	return read_size;
}

ssize_t CBufferWrite(circ_buffer_t *dst, const void *src, size_t count)
{
	size_t place_to_write = (dst->read_offset + dst->size) % dst->capacity;
	size_t write_size = count < CBufferFreeSpace(dst) ? count : 
														CBufferFreeSpace(dst);
	size_t how_much_to_write = 
					(place_to_write + write_size) < dst->capacity ?
					write_size :
					write_size - (place_to_write + write_size) % dst->capacity;
	const void *iter_src = src;
	
	assert(NULL != src);
	assert(NULL != dst);
	assert(0 != count);
	assert(dst->size <= dst->capacity);
	
	if(NO_FREE_SPACE == CBufferFreeSpace(dst))
	{
		errno = ENOSPC;
		return ERROR;
	}
	memcpy((void *)&dst->buffer[place_to_write], iter_src, how_much_to_write);
	iter_src = (const void *)((const char *)iter_src + how_much_to_write);
	how_much_to_write = write_size - how_much_to_write;
	memcpy((void *)dst->buffer, iter_src, how_much_to_write);
	dst->size += write_size;
	
	return write_size;
}

int CBufferIsEmpty(circ_buffer_t *buf)
{
	assert(NULL != buf);
	
	return (EMPTY == buf->size);
}

size_t CBufferSize(circ_buffer_t *buf)
{
	assert(NULL != buf);
	
	return buf->size;
}

size_t CBufferFreeSpace(circ_buffer_t *buf)
{
	assert(NULL != buf);
	
	return (buf->capacity - buf->size);
}
