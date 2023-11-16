#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>

typedef struct stack stack_t;

/* DESCRIPTION:
 * Create new stack
 *
 * @param:
 * capasity 	the max capasity of the stack 
 *
 * @return:
 * new stack
 */
stack_t *StackCreate(size_t capacity);

/* DESCRIPTION:
 * Destroy the stack
 *
 * @param:
 * stack 		pointer to the stack object 
 */
void StackDestroy(stack_t *stack);

/* DESCRIPTION:
 * Return the last element and remove it from the stack
 *
 * @param:
 * stack 		pointer to the stack object
 */
void StackPop(stack_t *stack);

/* DESCRIPTION:
 * Add new element to the stack
 *
 * @param:
 * stack 		pointer to the stack object 
 * element		element to be added to the stack
 */
void StackPush(stack_t *stack, void *element);

/* DESCRIPTION:
 * Return the last element
 *
 * @param:
 * stack 		pointer to the stack object 
 *
 * @return:
 * the last element
 */
void *StackPeek(const stack_t *stack);

/* DESCRIPTION:
 * Return the number of elements stored in the stack
 *
 * @param:
 * stack 		pointer to the stack object 
 *
 * @return:
 * the number of elements stored in the stack
 */
size_t StackSize(const stack_t *stack);

/* DESCRIPTION:
 * Checks if there is elements in the stack or not
 *
 * @param:
 * stack 		pointer to the stack object 
 *
 * @return:
 * 0 if there is elements in the stack, 1 otherwise
 */
int StackIsEmpty(const stack_t *stack);

/* DESCRIPTION:
 * Return the capacity of the stack
 *
 * @param:
 * stack 		pointer to the stack object 
 *
 * @return:
 * the capacity of the stack
 */
size_t StackCapacity(const stack_t *stack);

#endif /* __STACK_H__ */
