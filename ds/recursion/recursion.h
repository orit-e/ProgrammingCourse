/******************************************************************************
	
 						File name: 		recursion.h		
 						Written by: 	Orit		
 						Reviewed by: 	Liron		
 						Last edit: 		16.09.2020	
 								
 ******************************************************************************/
#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h> /* size_t */

typedef struct node node_t;
struct node
{
	void *data;
	node_t *next;
};

int FibonacciIter(int element_index);

int FibonacciRec(int element_index);

node_t *FlipList(node_t *node);

stack_t *StackSort(stack_t *stack, size_t stack_size);

size_t Strlen(const char *str);

int Strcmp(const char *str1, const char *str2);

char *Strcpy(char *dst, const char *src);

char *Strcat(char *dst, const char *src);

char *Strstr(const char *haystack, const char *needle);

#endif /* __RECURSION_H__ */