#ifndef __FSA_H__
#define __FSA_H__

typedef struct fsa fsa_t;

/* DESCRIPTION:
 * FSAInit() function receives memory_segment pointer to a segment of 
 * segment_size and divide it to blocks of block_size bytes. If block_size is 
 * not a multiple of the computer's word-size, the function will add the needed 
 * bytes so block_size will be a multiple of the computer's word-size. (Please 
 * see Notes section for more details.)
 * The function perform with complexity of O(n).
 * 
 * @param:
 * memory_segment	pointer to the memory segment FSA will now manage.
 * segment_size		size of given memory segment.
 * block_size		wanted size of each block for future allocation.
 *
 * Return value:
 * FSAInit() function return a pointer to fsa memory managar.
 *
 * Notes:
 * *FSAInit() only handles with blocks of the computer's word-size multiples. If
 *  block_size is not a multiple of the computer's word-size, the function will 
 *  add the needed bytes so block_size will be a multiple of the computer's 
 *  word-size.
 * *Please do not send NULL as memory_segment starting address, computer-wize 
 *  that is not allowed and will lead to undefined behavior.
 * *Please do not send 0 as block_size for that is illogical and will lead to 
 *  undefined behavior.
 * *If FSAInit() function nears the end of memory_segment of segment_size, and
 *  sees that the last few memory addresses do not fit a block of block_size,
 *  those addresses will be considered wasted an will cause an internal 
 *  fragmantation.
 * *User should send a segment_size large enough to contain at least one memory 
 *  block, or else the whole process is pointless.
 * *Please advise FSASuggestSegmentSize() function for the right size of 
 *  memory segment for your memory needs, though this function does not take 
 *  into account missalignment of first address of memory_segment (a thing that
 *  will cause memort area waste).
 */
fsa_t *FSAInit(void *memory_segment, size_t segment_size, size_t block_size);

/* DESCRIPTION:
 * FSAFree() function receives fsa memory managar and block to be freed by the 
 * function. (Please see Notes section for more details.)
 * The function perform with complexity of O(1).
 * 
 * @param:
 * fsa			 	pointer to the memory segment manager.
 * block			pointer to the block to be freed.
 *
 * Notes:
 * *Please do not send NULL as fsa or block starting address, computer-wize that
 *  is not allowed and will lead to undefined behavior.
 * *Please do not send through fsa variable an address other than the one given 
 *  back to the user by the FSAInit() function. Such a thing will lead to 
 *  undefined behavior.
 * *Please do not send through block variable an address other than the ones 
 *  given to the user by the FSAAlloc() function. Such a thing will lead to 
 *  undefined behavior.
 * *Please do not send through block variable an address outside of the memory 
 *  segment parameters. Such a thing will lead to undefined behavior.
 */
void FSAFree(fsa_t *fsa, void *block);

/* DESCRIPTION:
 * FSAAlloc() function receives fsa memory managar and return an allocated block
 * of block_size (see Notes and FSAInit() function description for details). 
 * The function perform with complexity of O(1).
 * 
 * @param:
 * fsa			 	pointer to the memory segment manager.
 *
 * Return value:
 * FSAAlloc() function return a pointer to allocated block of block_size.
 * If one is currently uavailable, the function will return a pointer to NULL.
 *
 * Note:
 * *Please do not send NULL as fsa starting address, computer-wize that
 *  is not allowed and will lead to undefined behavior.
 * *Please do not send through fsa variable an address other than the one given 
 *  back to the user by the FSAInit() function. Such a thing will lead to 
 *  undefined behavior.
 */
void *FSAAlloc(fsa_t *fsa);

/* DESCRIPTION:
 * FSACountFree() function receives fsa memory managar and counts the number of
 * blocks still available for future allocations in the given memory segment.
 * (Please see Notes section for more details.)
 * The function perform with complexity of O(n).
 * 
 * @param:
 * fsa			 	pointer to the memory segment manager.
 *
 * Return value:
 * FSACountFree() function return the number of blocks still available for 
 * future allocations in the given memory segment.
 *
 * Note:
 * *Please do not send NULL as fsa starting address, computer-wize that
 *  is not allowed and will lead to undefined behavior.
 * *Please do not send through fsa variable an address other than the one given 
 *  back to the user by the FSAInit() function. Such a thing will lead to 
 *  undefined behavior.
 */
size_t FSACountFree(const fsa_t *fsa);

/* DESCRIPTION:
 * FSASuggestSegmentSize() function receives desired block_size and 
 * num_of_blocks and suggest a memory segment size in bytes to accommodate such 
 * needs. If block_size is not a multiple of the computer's word-size, the 
 * function will add the needed bytes so block_size will be a multiple of the 
 * computer's word-size. The function does not take into account issues of 
 * alignment. (Please see Notes section for more details.)
 * The function perform with complexity of O(1).
 * 
 * @param:
 * block_size		wanted size of each block for future allocation.
 * num_of_blocks	wanted number of blocks for future allocation.
 *
 * Return value:
 * FSASuggestSegmentSize() function return a suggested memory segment size in 
 * bytes.
 *
 * Notes:
 * *FSAInit() function only handles with blocks of the computer's word-size 
 *  multiples. In order to give a better suggestion, if block_size is not a 
 *  multiple of the computer's word-size, FSASuggestSegmentSize() function will 
 *  add the needed bytes so block_size will be a multiple of the computer's 
 *  word-size in the function's inner calculation.
 * *Please do not send 0 as block_size or num_of_blocks for that is illogical 
 *  and will lead to undefined behavior.
 * *FSASuggestSegmentSize() function does not take into account memory waste due
 *  to missalignment of the first address of memory segment given to FSAInit() 
 *  function. User discretion is addvised.
 */
size_t FSASuggestSegmentSize(size_t block_size, size_t num_of_blocks);

#endif /* __FSA_H__ */
