/********************************************
 											
 		filename:			vsa.h			
 		written by:     	Orit			
 		last edit:      	26.08.2020		
 											
 ********************************************/
#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /*size_t*/

typedef struct vsa vsa_t;

/* DESCRIPTION:
 * VSAInit() function receives memory_segment of segment_size in order to manage
 * future allocations for the user from the given segment. If segment_size is 
 * not a multiple of the computer's word-size, the function will decrease the 
 * needed amount of bytes so the size of the segment aligible for allocation 
 * will be a multiple of the computer's word-size. (Please see Warnings section 
 * for more details.)
 * The function performs with complexity of O(n).
 * 
 * @param:
 * memory_segment	pointer to the memory segment VSA will now manage.
 * segment_size		size of given memory segment.
 *
 * Return value:
 * VSAInit() function return a pointer to vsa memory managar.
 *
 * Warnings:
 * *VSAInit() only handles with segments of the computer's word-size multiples. 
 *  If the size of the aligible segment for future allocations (meaning, 
 *  segment_size minus manager size and minus bytes wasted due to misalignment 
 *  of memory_segment's first address) is not a multiple of the computer's 
 *  word-size, the function will decrease the needed bytes so this segment will 
 *  be a multiple of the computer's word-size.
 * *Do not send NULL as memory_segment starting address, such a thing will lead 
 *  to undefined behavior.
 * *Do not send 0 as segment_size, such a thing will lead to undefined behavior.
 * *User should send a segment_size at least larger than 3 word-size for  
 *  allocation of minimal size to be possible, though it is advised to send a 
 *  segment_size much bigger than that.
 */
vsa_t *VSAInit(void *memory_segment, size_t segment_size);

/* DESCRIPTION:
 * VSAFree() function receives allocated chunk to be freed by the function. 
 * (Please see Warnings section for more details.)
 * The function performs with complexity of O(1).
 * 
 * @param:
 * chunk			pointer to allocated chunk to be freed.
 *
 * Warnings:
 * *If chunk is NULL, no operation is performed.
 * *Do not send through chunk variable an address other than the ones given to 
 *  the user by the VSAAlloc() function. Such a thing will lead to undefined 
 *  behavior.
 * *Do not send through chunk variable an address outside of the memory segment 
 *  parameters. Such a thing will lead to undefined behavior.
 */
void VSAFree(void *chunk);

/* DESCRIPTION:
 * VSAAlloc() function receives vsa memory managar and chunk_size, and return an
 * allocated chunk of chunk_size aligned to word-size if one is currently 
 * available. (see Warnings and VSAInit() function description for details). 
 * The function performs with complexity of O(n).
 * 
 * @param:
 * vsa			 	pointer to the memory segment manager.
 * chunk_size		desired size of chunk to be allocated.
 *
 * Return value:
 * VSAAlloc() function return a pointer to allocated chunk of chunk_size.
 * If one is currently uavailable, the function will return a pointer to NULL.
 *
 * Warnings:
 * *Please do not send NULL as vsa starting address, computer-wize that
 *  is not allowed and will lead to undefined behavior.
 * *Do not send through vsa variable an address other than the one given back to
 *  the user by the VSAInit() function. Such a thing will lead to undefined 
 *  behavior.
 * *Do not send 0 as chunk_size, such a thing will lead to undefined behavior.
 * *If chunk_size is not a multiple of the computer's word-size, the function 
 *  will increase the needed amount of bytes so chunk_size will be a multiple of
 *  the computer's word-size.
 */
void *VSAAlloc(vsa_t *vsa, size_t chunk_size);

/* DESCRIPTION:
 * VSALargestChunkAvailable() function receives vsa memory managar and return 
 * the size of the largest Contiguous memory chunk available. (see Warnings and 
 * VSAInit() function description for details).
 * The function performs with complexity of O(n).
 * 
 * @param:
 * vsa			 	pointer to the memory segment manager.
 *
 * Return value:
 * VSALargestChunkAvailable() function return the size of the largest Contiguous
 * memory chunk available in bytes. If the entire memory segment managed by vsa 
 * is allocated and uavailable, the function will return 0.
 *
 * Warnings:
 * *Do not send NULL as vsa starting address, such a thing will lead to 
 *  undefined behavior.
 * *Do not send through vsa variable an address other than the one given back to
 *  the user by the VSAInit() function. Such a thing will lead to undefined 
 *  behavior.
 */
size_t VSALargestChunkAvailable(vsa_t *vsa);

#endif /*__VSA_H__*/
