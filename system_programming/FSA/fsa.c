/********************************************
 *											*
 *		filename:			fsa.c			*
 *		written by:     	Orit			*
 *		reviewed by:    	Renee			*
 *		last edit:      	20.08.2020		*
 *											*
 ********************************************/
#include <stdlib.h>	/* size_t	*/
#include <assert.h>	/* assert()	*/
#include "fsa.h"

typedef struct block_header block_header_t;

struct fsa
{
	block_header_t *first_free_block;
};

struct block_header
{
	block_header_t *next_free;
};

static size_t BlockSizeAlignedUpIMP(size_t block_size)
{
	size_t word_size = sizeof(size_t);
	
	assert(0 != block_size);
	
	return (block_size + (word_size - block_size % word_size) % word_size);
}

static size_t NumOfBlocksIMP(size_t segment_size, 
							size_t segment_alignment, 
							size_t block_size)
{
	size_t size_of_fsa = sizeof(fsa_t);
	
	assert(0 != block_size);
	assert(segment_alignment + size_of_fsa + block_size <= segment_size);
	
	return ((segment_size - segment_alignment - size_of_fsa) / block_size);
}

fsa_t *FSAInit(void *memory_segment, size_t segment_size, size_t block_size)
{
	fsa_t *new_fsa = NULL;
	block_header_t *iter_block = NULL;
	size_t segment_alignment = (size_t)memory_segment % sizeof(size_t);
	size_t size_of_fsa = sizeof(*new_fsa);
	size_t num_of_blocks = 0;
	
	assert(NULL != memory_segment);
	assert(0 != block_size);
	assert(segment_alignment + size_of_fsa + block_size <= segment_size);
	
	new_fsa = (fsa_t *)memory_segment;
	block_size = BlockSizeAlignedUpIMP(block_size);
	num_of_blocks = NumOfBlocksIMP(segment_size, segment_alignment, block_size);
	new_fsa->first_free_block = (block_header_t *)(new_fsa + 1);
	iter_block = new_fsa->first_free_block;
	while(0 < --num_of_blocks)
	{
		iter_block->next_free = (block_header_t *)((char *)iter_block + 
													block_size);
		iter_block = iter_block->next_free;
	}
	iter_block->next_free = NULL;
	
	return new_fsa;
}

void *FSAAlloc(fsa_t *fsa)
{
	block_header_t *first_free_block = NULL;
	
	assert(NULL != fsa);
	
	if(NULL == fsa->first_free_block)
	{
		return NULL;
	}
	first_free_block = fsa->first_free_block;
	fsa->first_free_block = first_free_block->next_free;
	first_free_block->next_free = NULL;
	
	return (void *)(first_free_block);
}

void FSAFree(fsa_t *fsa, void *block)
{
	block_header_t *block_header_to_free = NULL;
	
	assert(NULL != fsa);
	assert(NULL != block);
	
	block_header_to_free = (block_header_t *)block;
	block_header_to_free->next_free = fsa->first_free_block;
	fsa->first_free_block = block_header_to_free;
}

size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	block_header_t *iter_block_header = NULL;
	
	assert(NULL != fsa);
	
	iter_block_header = fsa->first_free_block;
	while(NULL != iter_block_header)
	{
		iter_block_header = iter_block_header->next_free;
		++counter;
	}
	
	return counter;
}

size_t FSASuggestSegmentSize(size_t block_size, size_t num_of_blocks)
{
	assert(0 != block_size);
	assert(0 != num_of_blocks);
	
	return (sizeof(fsa_t) + BlockSizeAlignedUpIMP(block_size) * num_of_blocks);
}

