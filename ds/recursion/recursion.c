/******************************************************************************
	
 						File name: 		recursion.c		
 						Written by: 	Orit		
 						Reviewed by: 	Liron		
 						Last edit: 		16.09.2020	
 								
 ******************************************************************************/
#include <assert.h> /* assert() */

#include "stack.h"
#include "recursion.h"

int FibonacciIter(int element_index)
{
    int i = 0;
    int fibo_element = 0;
    int last_element = 0;
    int next_element = 1;

    for(i = 0; element_index > i; ++i)
    {
        last_element = fibo_element;
        fibo_element += next_element;
        next_element = last_element;
    }
        
    return fibo_element;
}

int FibonacciRec(int element_index)
{
    if(0 ==  element_index || 1 == element_index)
    {
        return element_index;
    }
        
    return (FibonacciRec(element_index - 1) + FibonacciRec(element_index - 2));
}

node_t *FlipList(node_t *node)
{
    node_t *new_head = NULL;

    assert(NULL != node);

    if(NULL == node->next)
    {
        return node;
    }
    new_head = FlipList(node->next);
    node->next->next = node;
    node->next = NULL;
    
    return new_head;
}

static void SwapNums(int **num1, int **num2)
{
    size_t temp = 0;

    assert(NULL != num1);
    assert(NULL != num2);

    temp = (size_t)*num1;
    *num1 = *num2;
    *num2 = (int *)temp;
}

static int *StackPeekAndPop(stack_t *stack)
{
    int *num = NULL;

    assert(NULL != stack);

    num = (int *)StackPeek(stack);
    StackPop(stack);

    return num;
}

static int *NumsPopAndSort(stack_t *stack, size_t n)
{
    int *num1 = 0;
    int *num2 = 0;

    assert(NULL != stack);

    if(1 == n)
    {
        return StackPeekAndPop(stack);
    }
    num1 = StackPeekAndPop(stack);
    num2 = NumsPopAndSort(stack, --n);
    if(*num2 > *num1)
    {
        SwapNums(&num1, &num2);
    }
    StackPush(stack, (void *)num2);

    return num1;
}

stack_t *StackSort(stack_t *stack, size_t stack_size)
{
    int *num = 0;

    assert(NULL != stack);

    if(0 != stack_size && 1 != stack_size)
    {
        StackSort(stack, stack_size - 1);
        num = (int *)NumsPopAndSort(stack, stack_size);
        StackPush(stack, (void *)num);
    }
        
    return stack;
}

size_t Strlen(const char *str)
{
    assert(NULL != str);

    if('\0' ==  *str)
    {
        return 0;
    }
        
    return (1 + Strlen(++str));
}

int Strcmp(const char *str1, const char *str2)
{
    assert(NULL != str1);
    assert(NULL != str2);

    if('\0' == *str1 || *str1 != *str2)
    {
        return (*str1 - *str2);
    }
        
    return Strcmp(++str1, ++str2);
}

char *Strcpy(char *dst, const char *src)
{
    assert(NULL != src);
    assert(NULL != dst);

    if('\0' != *src)
    {
        Strcpy(dst + 1, src + 1);
    }
    *dst = *src;

    return dst;
}

char *Strcat(char *dst, const char *src)
{
    assert(NULL != src);
    assert(NULL != dst);

    if('\0' == *dst)
    {
         return Strcpy(dst, src);   
    }

    return Strcat(dst + 1, src) - 1;
}

static int Strncmp(const char *str1, const char *str2, size_t num)
{	
	assert(NULL != str1);
    assert(NULL != str2);

    if('\0' == *str1 || *str1 != *str2 || 1 == num)
    {
        return (*str1 - *str2);
    }
        
    return Strncmp(++str1, ++str2, --num);
}

char *Strstr(const char *haystack, const char *needle)
{
    size_t needle_len = 0;

    assert(NULL != haystack);
    assert(NULL != needle);

    needle_len = Strlen(needle);
    if('\0' == *haystack || 0 == Strncmp(haystack, needle, needle_len))
    {
        return (char *)haystack;
    }
    
    return Strstr(haystack + 1, needle);
}
