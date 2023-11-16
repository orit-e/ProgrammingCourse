/********************************************
 *											*
 *		filename:			vector.h		*
 *		written by:     	Orit			*
 *		reviewed by:    	Moris			*
 *		last edit:      	29.07.2020		*
 *											*
 ********************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

typedef struct vector vector_t;

/* DESCRIPTION:
 * creates a dynamic vector according to given capacity.
 *
 * @param:
 * capacity 	 initial capacity of vector.
 *
 * @return:
 * pointer to created vector. 
 * 
 */
vector_t *VectorCreate(size_t capacity);

/* DESCRIPTION:
 * destroy the vector.
 *
 * @param:
 * vector 		pointer to the vector. 
 */
void VectorDestroy(vector_t *vector);

/* DESCRIPTION:
 * add new element to end of vector.
 *
 * @param:
 * vector 		pointer to vector.
 * element		element to be added to vector.
 * 
 * @return:
 * 0 is returned in case there is memory left for the next element.
 * 1 is returned in case memory ran out after successful push of given element.
 */
int VectorPush(vector_t *vector, void *element);

/* DESCRIPTION:
 * remove last element from vector.
 *
 * @param:
 * 0 is returned in case memory shrinkage occured.
 * 1 is returned in case memory shrinkage did not occur.
 */
int VectorPop(vector_t *vector);

/* DESCRIPTION:
 * return the number of elements stored in the vector
 *
 * @param:
 * vector		pointer to the vector 
 *
 * @return:
 * number of elements stored in vector
 */
size_t VectorSize(const vector_t *vector);

/* DESCRIPTION:
 * returns capacity of vector
 *
 * @param:
 * vector 		pointer to the vector 
 *
 * @return:
 * capacity of vector
 */
size_t VectorCapacity(const vector_t *vector);

/* DESCRIPTION:
 * returns  pointer to data stored in a given index.
 *
 * @param:
 * vector 		pointer to the vector.
 * index		index of element to get.
 *
 * @return:
 * pointer to data stored in index.
 */
void *VectorGetElement(const vector_t *vector, size_t index);

/* DESCRIPTION:
 * sets a value to given index.
 *
 * @param:
 * vector 		pointer to the vector.
 * index		index of element to set.
 * element 		element to set.
 */
void VectorSetElement(const vector_t *vector, size_t index, void *element);

/* DESCRIPTION:
 * expands vector.
 *
 * @param:
 * vector 		pointer to the vector.
 * capacity		new capacity (must be bigger than current capacity).
 * 
 * @return:
 * 0 for success 1 for failure.
 */
int VectorExpand(vector_t *vector, size_t capacity);

/* DESCRIPTION:
 * shrinks vector capacity to current vector size.
 *
 * @param:
 * vector 		pointer to the vector.
 *
 * @return:
 * 0 for success 1 for failure.
 */
int VectorShrink(vector_t *vector);

#endif /* __VECTOR_H__ */
