/********************************************
 *											*
 *		filename:			vector.c		*
 *		written by:     	Orit			*
 *		reviewed by:    	Moris			*
 *		last edit:      	29.07.2020		*
 *											*
 ********************************************/
#include <stdlib.h>	/* malloc(), realloc(), free(), size_t 	*/
#include <assert.h>	/* assert() 							*/
#include <string.h>	/* memset() 							*/

#include "vector.h"

/******************************************************************************/
/*							defines and macro function:						  */
#define TRUE 1
#define GROWTH_FACTOR 2
#define DOUBLE_GROWTH_FACTOR 4
#define MIN_CAPACITY 8

#define IS_RESIZE(bool_term, vector, new_capacity, is_realloc_success) \
{\
	if(TRUE == bool_term)\
	{\
		is_realloc_success = VectorResize(vector, new_capacity);\
	}\
}\

/******************************************************************************/
/*						enum and struct declarations:						  */
enum IS_SUCCESS
{
	VECTOR_SUCCESS = 0, 
	VECTOR_FAIL
};

struct vector
{
	void **members;
	size_t size;
	size_t capacity;
};

/******************************************************************************/
/*		        		Static function declarations:		        		  */
/* One function to realloc them all! */
static int VectorResize(vector_t *vector, size_t new_capacity);

/******************************************************************************/
/*								Vector functions:							  */
vector_t *VectorCreate(size_t capacity)
{
	vector_t *new_vector = (vector_t *)malloc(sizeof(vector_t));
	
	assert(0 < capacity);
	
	if(NULL == new_vector)
	{
		return NULL;
	}
	if(capacity < MIN_CAPACITY)
	{
		capacity = MIN_CAPACITY;
	}
	new_vector->members = (void **)malloc(sizeof(void *) * capacity);
	if(NULL == new_vector->members)
	{
		free(new_vector);
		return NULL;
	}
	new_vector->capacity = capacity;
	new_vector->size = 0;
	
	return new_vector;
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	
	memset(vector->members, 0, vector->capacity);
	free(vector->members);
	memset(vector, 0, sizeof(vector_t));
	free(vector);
}

int VectorPush(vector_t *vector, void *element)
{
	int is_realloc_success = VECTOR_SUCCESS;
	
	assert(NULL != vector);
	assert(NULL != element);
	assert(vector->size < vector->capacity);
	
	vector->members[vector->size] = element;
	vector->size += 1;
	IS_RESIZE((vector->size == vector->capacity), vector, 
				(vector->capacity * GROWTH_FACTOR), is_realloc_success);
	
	return is_realloc_success;
}

int VectorPop(vector_t *vector)
{
	int is_realloc_success = VECTOR_SUCCESS;
	
	assert(NULL != vector);
	assert(0 < vector->size);
	
	vector->size -= 1;
	IS_RESIZE(((vector->size * DOUBLE_GROWTH_FACTOR) <= vector->capacity), 
				vector, (vector->capacity / GROWTH_FACTOR), is_realloc_success);
	
	return is_realloc_success;
}

size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);

	return vector->size;
}

size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);

	return vector->capacity;	
}

void *VectorGetElement(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	assert(index < vector->size);
	
	return vector->members[index];
}

void VectorSetElement(const vector_t *vector, size_t index, void *element)
{
	assert(NULL != vector);
	assert(NULL != element);
	assert(index < vector->size);
	
	vector->members[index] = element;
}

int VectorExpand(vector_t *vector, size_t capacity)
{
	
	assert(NULL != vector);
	if(vector->capacity > capacity)
	{
		return VECTOR_FAIL;
	}
	
	return VectorResize(vector, capacity);
}

int VectorShrink(vector_t *vector)
{
	assert(NULL != vector);
	
	return VectorResize(vector, vector->size + 1);
}

/******************************************************************************/
/*		        		Static function definitions:		        		  */
static int VectorResize(vector_t *vector, size_t new_capacity)
{
	void **temp_ptr = vector->members;
	
	assert(NULL != vector);
	assert(0 < new_capacity);
	
	if(MIN_CAPACITY > new_capacity)
	{
		new_capacity = MIN_CAPACITY;
	}
	temp_ptr = (void **)realloc(temp_ptr, sizeof(void *) * new_capacity);
	if(NULL == temp_ptr)
	{
		return VECTOR_FAIL;
	}
	vector->members = temp_ptr;
	vector->capacity = new_capacity;
	
	return VECTOR_SUCCESS;
}
