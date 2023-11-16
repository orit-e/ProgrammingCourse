/************************************************
 *												*
 *		filename:			list_exercises.c	*
 *		written by:     	Orit				*
 *		reviewed by:    	Dor					*
 *		last edit:      	3.8.2020			*
 *												*
 ************************************************/

#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc, realloc, free, size_t */
#include <assert.h>	/* assert */
#include <string.h>	/* memset, strcmp */

/******************************************************************************/
/* Macros and enum for tests: */
#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"
#define BLUE_COLOR		"\x1b[0;34m"

enum EXIT_STATUS {FAILED, SUCCESS, DONE};

#define RUN_TEST(result, name) {\
			printf("test %s: ", name);\
			switch (result)\
			{\
				case SUCCESS:\
					printf(GREEN_COLOR);\
					printf("Success\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case FAILED:\
					printf(RED_COLOR);\
					printf("Fail\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case DONE:\
					printf(BLUE_COLOR);\
					printf("Done\n");\
					printf(DEFAULT_COLOR);\
			};\
		}

/******************************************************************************/
/* Declaration of struct type and enum for exercise function: */
typedef struct node
{
	void *data;
	struct node *next;
}node_t;

enum {NO_LOOP = 0, LOOP};

/******************************************************************************/
/* Functions declarations: */

/* Exercise functions decleration: */

/* DESCRIPTION:
 * Reverses the order of given slist.
 *
 * @param:
 * head		head of slist.
 *
 * @return:
 * New head of slist after Flip.
 */
node_t *Flip(node_t *head);

/* DESCRIPTION:
 * Tells whether a given slist has loop.
 *
 * @param:
 * head		head of slist.
 *
 * @return:
 * 1 if has loop, 0 otherwise.
 */
int HasLoop(const node_t *head);

/* DESCRIPTION:
 * Returns a pointer to the first node mutual to both head_1 and head_2 slists,
 * if any.
 *
 * @param:
 * head_1	head of first slist.
 * head_2	head of second slist.
 *
 * @return:
 * pointer to first node mutual to both head_1 and head_2 slists if exist, NULL
 * otherwise.
 */
node_t *FindIntersection(node_t *head_1, node_t *head_2);

/* Helping functions to count how many nodes in list: */
size_t SlistCount(const node_t *iter);

/* Helping function to move head forward given steps times. */
node_t *SlistForward(node_t *head, int steps);

/* test functions declarations: */
int TestFlip();
int TestHasLoop();
int TestFindIntersection();
static void Init();
static void Destroy();

/******************************************************************************/
/* global variables for tests: */
node_t *head = NULL;
node_t *head2 = NULL;
int d1 = 1, d2 = 2, d3 = 3, d4 = 4;

/******************************************************************************/
int main(int argc, char *argv[])
{
	Init();
	RUN_TEST(TestFlip(),				"Flip");
	RUN_TEST(TestHasLoop(),				"HasLoop");
	RUN_TEST(TestFindIntersection(),	"FindIntersection");
	Destroy();

	(void) argc;
	(void) argv;

    return 0;
}

/******************************************************************************/
/* Exercise functions: */
node_t *Flip(node_t *head)
{
	node_t *iter_before = head;
	node_t *iter = head;
	node_t *iter_next = head->next;
	
	assert(NULL != head);
	
	iter_before->next = NULL;
	while(NULL !=  iter_next)
	{
		iter = iter_next;
		iter_next = iter->next;
		iter->next = iter_before;
		iter_before = iter;
	}
	
	return iter;
}

int HasLoop(const node_t *head)
{
	const node_t *slow_iter = head;
	const node_t *fast_iter = head->next;
	
	assert(NULL != head);
	
	while(NULL != fast_iter && NULL != fast_iter->next && 
			slow_iter != fast_iter)
	{
		slow_iter = slow_iter->next;
		fast_iter = fast_iter->next->next;
	}
	
	return (slow_iter == fast_iter);
}

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
	
	node_t *iter_1 = head_1;
	node_t *iter_2 = head_2;
	int size_diff = SlistCount(iter_1) - SlistCount(iter_2);
	
	assert(NULL != head_1);
	assert(NULL != head_2);
	
	if(0 < size_diff)
	{
		iter_1 = SlistForward(iter_1, size_diff);
	}
	else
	{
		iter_2 = SlistForward(iter_2, -size_diff);
	}
	while(iter_1 != iter_2)
	{
		iter_1 = iter_1->next;
		iter_2 = iter_2->next;
	}
	
	return iter_1;
}
/******************************************************************************/
/* helping functions: */
size_t SlistCount(const node_t *iter)
{
	size_t counter = 0;
	
	assert(NULL != iter);
	
	while(NULL != iter)
	{
		++counter;
		iter = iter->next;
	}
	
	return counter;
}

node_t *SlistForward(node_t *head, int steps)
{
	node_t *iter = head;
	
	assert(NULL != head);
	
	while(0 < steps--)
	{
		iter = iter->next;
	}
	
	return iter;
}

/*****************************************************************************/
/* Test functions: */
void Init()
{
	node_t *node2 = (node_t *)malloc(sizeof(node_t));
	node_t *node3 = (node_t *)malloc(sizeof(node_t));
	node_t *node4 = (node_t *)malloc(sizeof(node_t));
	head = (node_t *)malloc(sizeof(node_t));

	head->data = (void *)&d1;
	node2->data = (void *)&d2;
	node3->data = (void *)&d3;
	node4->data = (void *)&d4;

	head->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = NULL;
}

static void Destroy()
{
	node_t *curr = head2;
	node_t *next = curr->next;
	
	while(NULL != next)
	{
		free(curr);
		curr = next;
		next = curr->next;
	}
	free(curr);
	free(head);
}

int TestFlip()
{
	head = Flip(head);
	return (4 == *(int*)head->data);
}

int TestHasLoop()
{
	int result = 0;
	result += HasLoop((const node_t *)head);
	head->next->next->next->next = head;
	result += HasLoop((const node_t *)head);

	return (1 == result);
}

int TestFindIntersection()
{
	node_t *node2 = (node_t *)malloc(sizeof(node_t));
	node_t *node3 = (node_t *)malloc(sizeof(node_t));
	node_t *node4 = (node_t *)malloc(sizeof(node_t));
	head2 = (node_t *)malloc(sizeof(node_t));

	head2->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = head->next;
	head->next->next->next->next = NULL;

	return(head->next == FindIntersection(head, head2));
}
