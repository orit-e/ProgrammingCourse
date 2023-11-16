/********************************************
 *											*
 *		filename:			vsa.c			*
 *		written by:     	Orit			*
 *		reviewed by:    	Rotem			*
 *		last edit:      	26.08.2020		*
 *											*
 ********************************************/
#include <stdlib.h>	/* labs()	*/
#include <assert.h>	/* assert()	*/

#include "vsa.h"

#define VSA_MAGIC_NUMBER 0xDEADBEEFDEADBEEF

struct vsa
{
	size_t offset_to_end;
};

typedef struct chunk_header
{
	long size;
	#ifndef NDEBUG
		size_t vsa_magic_number;
	#endif
}chunk_header_t;

/******************************************************************************/
/*Static functions declarations:*/
static size_t OffsetFromSegmentStartIMP(vsa_t *vsa, chunk_header_t *chunk);
static size_t AlignUpIMP(size_t to_align_up);
static size_t AlignDownIMP(size_t to_align_down);
static chunk_header_t *NextChunkIMP(chunk_header_t *chunk, size_t chunk_size);
static size_t UsableSegmentSizeIMP(void *memory_segment, size_t segment_size);
static long VSADeFragIMP(vsa_t *vsa, chunk_header_t *chunk);
static void *AllocCurrentChunkIMP(chunk_header_t *chunk);
#ifndef NDEBUG
	static size_t MinSizeOfSegmentIMP(void *memory_segment);
#endif
static long ReSizeChunkUPIMP(long old_size, long diff_to_new_size);
static long ReSizeChunkDownIMP(long old_size, long diff_to_new_size);
static void FlipFreeAndAllocModesIMP(chunk_header_t *chunk);

/******************************************************************************/
/*VSA exercise functions:*/
vsa_t *VSAInit(void *memory_segment, size_t segment_size)
{
	vsa_t *new_vsa = NULL;
	chunk_header_t *one_free_chunk = NULL;
	
	assert(NULL != memory_segment);
	assert(MinSizeOfSegmentIMP(memory_segment) < segment_size);
	
	new_vsa = (vsa_t *)AlignUpIMP((size_t)memory_segment);
	new_vsa->offset_to_end = UsableSegmentSizeIMP(memory_segment, segment_size);
	one_free_chunk = (chunk_header_t *)(new_vsa + 1);
	one_free_chunk->size = ReSizeChunkDownIMP(new_vsa->offset_to_end, 0);
	#ifndef NDEBUG
		one_free_chunk->vsa_magic_number = VSA_MAGIC_NUMBER;
	#endif
	
	return new_vsa;
}

void VSAFree(void *chunk)
{
	if(NULL != chunk)
	{
		chunk_header_t *chunk_to_free = (chunk_header_t *)((char *)chunk - 
														sizeof(*chunk_to_free));

		assert(VSA_MAGIC_NUMBER == chunk_to_free->vsa_magic_number);

		if(0 > chunk_to_free->size)
		{
			FlipFreeAndAllocModesIMP(chunk_to_free);
		}
	}
}

void *VSAAlloc(vsa_t *vsa, size_t chunk_size)
{
	chunk_header_t *chunk = NULL;
	void *chunk_to_alloc = NULL;
	size_t size_of_header = sizeof(*chunk);
	
	assert(NULL != vsa);
	assert(0 != chunk_size);
	
	chunk_size = AlignUpIMP(chunk_size);
	
	chunk = (chunk_header_t *)(vsa + 1);
	while(vsa->offset_to_end > OffsetFromSegmentStartIMP(vsa, chunk) && 
			(long)chunk_size > chunk->size)
	{
		chunk->size = VSADeFragIMP(vsa, chunk);
		if(vsa->offset_to_end > OffsetFromSegmentStartIMP(vsa, chunk) && 
			(long)chunk_size > chunk->size)
		{
			chunk = NextChunkIMP(chunk, labs(chunk->size));
		}
	}
	if((long)chunk_size <= chunk->size && 
		(long)(chunk_size + size_of_header) >= chunk->size)
	{
		chunk_to_alloc = AllocCurrentChunkIMP(chunk);
	}
	if((long)(chunk_size + size_of_header) < chunk->size)
	{
		chunk_header_t *next_chunk = NextChunkIMP(chunk, chunk_size);
		next_chunk->size = ReSizeChunkDownIMP(chunk->size, chunk_size);
		chunk->size = ReSizeChunkDownIMP(chunk->size, next_chunk->size);
		chunk_to_alloc = AllocCurrentChunkIMP(chunk);
	}
	
	return chunk_to_alloc;	
}

size_t VSALargestChunkAvailable(vsa_t *vsa)
{
	chunk_header_t *chunk = NULL;
	long largest_chunk_available = 0;
	
	assert(NULL != vsa);
	
	chunk = (chunk_header_t *)(vsa + 1);
	do
	{
		chunk->size = VSADeFragIMP(vsa, chunk);
		largest_chunk_available = (largest_chunk_available > chunk->size) ?
									largest_chunk_available : chunk->size;
		if(vsa->offset_to_end > OffsetFromSegmentStartIMP(vsa, chunk))
		{
			chunk = NextChunkIMP(chunk, labs(chunk->size));
		}
	}
	while(vsa->offset_to_end > OffsetFromSegmentStartIMP(vsa, chunk));
	largest_chunk_available = (largest_chunk_available > chunk->size) ?
								largest_chunk_available : chunk->size;
	
	
	return (size_t)(largest_chunk_available);
}

/******************************************************************************/
/*Static functions for VSA exercise:*/
static size_t OffsetFromSegmentStartIMP(vsa_t *vsa, chunk_header_t *chunk)
{
	assert(NULL != vsa);
	assert(NULL != chunk);
	
	return ((size_t)(chunk + 1) - (size_t)(vsa + 1) + 
			labs(chunk->size));
}

static size_t AlignUpIMP(size_t to_align_up)
{
	size_t word_size = sizeof(size_t);
	
	assert(0 != to_align_up);
	
	return (to_align_up + (word_size - to_align_up % word_size) % word_size);
}

static size_t AlignDownIMP(size_t to_align_down)
{
	size_t word_size = sizeof(size_t);
	
	assert(0 != to_align_down);
	
	return (to_align_down - to_align_down % word_size);
}

static chunk_header_t *NextChunkIMP(chunk_header_t *chunk, size_t chunk_size)
{
	assert(NULL != chunk);
	assert(0 != chunk_size);
	
	return (chunk_header_t *)((char *)(chunk + 1) + chunk_size);
}

static size_t UsableSegmentSizeIMP(void *memory_segment, size_t segment_size)
{
	size_t offset_to_end = 0;
	
	assert(NULL != memory_segment);
	assert(MinSizeOfSegmentIMP(memory_segment) < segment_size);
	
	offset_to_end = segment_size - (AlignUpIMP((size_t)memory_segment) - 
					(size_t)memory_segment ) - sizeof(vsa_t);

	return (AlignDownIMP(offset_to_end));
}

static long VSADeFragIMP(vsa_t *vsa, chunk_header_t *chunk)
{
	chunk_header_t *next_chunk = NULL;
	
	assert(NULL != vsa);
	assert(NULL != chunk);
	
	next_chunk = chunk;
	while(vsa->offset_to_end > OffsetFromSegmentStartIMP(vsa, chunk) && 
			0 < next_chunk->size)
	{
		next_chunk = NextChunkIMP(chunk, labs(chunk->size));
		if(0 < chunk->size && 0 < next_chunk->size)
		{
			chunk->size = ReSizeChunkUPIMP(chunk->size, next_chunk->size);
		}
	}
	
	return chunk->size;
}

static void *AllocCurrentChunkIMP(chunk_header_t *chunk)
{
	assert(NULL != chunk);
	
	FlipFreeAndAllocModesIMP(chunk);
	#ifndef NDEBUG
		chunk->vsa_magic_number = VSA_MAGIC_NUMBER;
	#endif
	
	return (void *)(chunk + 1);
}
#ifndef NDEBUG
static size_t MinSizeOfSegmentIMP(void *memory_segment)
{
	assert(NULL != memory_segment);
	
	return (sizeof(vsa_t) + sizeof(chunk_header_t) + 
			(AlignUpIMP((size_t)memory_segment) - (size_t)memory_segment) + 
			sizeof(size_t));
}
#endif

static long ReSizeChunkUPIMP(long old_size, long diff_to_new_size)
{
	return (old_size + diff_to_new_size + sizeof(chunk_header_t));
}

static long ReSizeChunkDownIMP(long old_size, long diff_to_new_size)
{
	return (old_size - diff_to_new_size - sizeof(chunk_header_t));
}

static void FlipFreeAndAllocModesIMP(chunk_header_t *chunk)
{
	assert(NULL != chunk);
	
	chunk->size *= (-1);
}

