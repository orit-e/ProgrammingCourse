/********************************
 *								*
 *	File name: 		stack.c		*
 *	Written by: 	Orit		*
 *	Reviewed by: 	Lidia		*
 *	Last edit: 		28.07.2020	*
 *								*
 ********************************/

#include <stdlib.h>	/* size_t, malloc, free */
#include <string.h> /* memset */
#include <assert.h>	/* assert */
#include "stack.h"

struct stack
{
	size_t size;
	size_t capacity;
	void **members;
};

stack_t *StackCreate(size_t capacity)
{
	stack_t *new_stack = (stack_t *)malloc(sizeof(stack_t) + 
							sizeof(void *) * capacity);
	
	assert(0 < capacity);
	
	if(NULL == new_stack)
	{
		return NULL;
	}
	new_stack->size = 0;
	new_stack->capacity = capacity;
	
	new_stack->members = (void **)((char *)new_stack + sizeof(stack_t));
	
	return new_stack;
}

void StackDestroy(stack_t *stack)
{	
	assert(NULL != stack);
	
	memset(stack->members, 0, stack->capacity);
	memset(stack, 0, sizeof(stack_t));
	free(stack);
	stack = NULL;
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	assert(0 < stack->size);
	
	stack->size -= 1;
}

void StackPush(stack_t *stack, void *element)
{
	assert(NULL != stack);
	assert(NULL != element);
	assert(stack->capacity > stack->size);
	
	stack->members[stack->size] = element;
	stack->size += 1;
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	assert(0 < stack->size);
	
	return stack->members[stack->size - 1];
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);

	return stack->size;
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);

	return (0 == StackSize(stack));
}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);

	return stack->capacity;	
}
