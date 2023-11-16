#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include "stack.h"

#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"

#define RUN_TEST(result, name) {\
			printf("test %s: ", name);\
			if (result)\
			{\
				printf(GREEN_COLOR);\
				printf("Success\n");\
				printf(DEFAULT_COLOR);\
			}\
			else\
			{\
				printf(RED_COLOR);\
				printf("Fail\n");\
				printf(DEFAULT_COLOR);\
			}\
		}

int TestStackCreate();
int TestStackDestroy();
int TestStackPop();
int TestStackPush();
int TestStackPeek();
int TestStackSize();
int TestStackIsEmpty();
int TestStackCapacity();

stack_t *stack= NULL;
int d = 10;
char c = 'a';
char *s = "hello";

int main(int argc, char *argv[])
{
	RUN_TEST(TestStackCreate(), 		"StackCreate" 	);
	RUN_TEST(TestStackPush(),			"StackPush"		);
	RUN_TEST(TestStackPeek(),			"StackPeek"	 	);
	RUN_TEST(TestStackPop(),			"StackPop" 		);
	RUN_TEST(TestStackSize(),			"StackSize"		);
	RUN_TEST(TestStackIsEmpty(),		"StackIsEmpty"	);
	RUN_TEST(TestStackCapacity(),		"StackCapacity"	);
	RUN_TEST(TestStackDestroy(),		"StackDestroy" 	);

	(void) argc;
	(void) argv;
	
	return 0;
}

int TestStackCreate()
{
	stack = StackCreate((size_t) 3);
	return 1;
}

int TestStackPush()
{
	StackPush(stack, (void*) &d);
	StackPush(stack, (void*) &c);
	StackPush(stack, (void*) s);
	return 1;
}

int TestStackPeek()
{
	return (0 == strcmp((char*)StackPeek(stack), "hello")); 
}

int TestStackPop()
{
	StackPop(stack);
	StackPop(stack);
	StackPop(stack);
	return 1;
}

int TestStackSize()
{
	StackPush(stack, (void*) &d);
	StackPush(stack, (void*) &c);
	return (2 == StackSize(stack));
}

int TestStackIsEmpty()
{
	return (0 == StackIsEmpty(stack));
}

int TestStackCapacity()
{
	return (3 == StackCapacity(stack));
}

int TestStackDestroy()
{
	StackDestroy(stack);
	stack = NULL;
	return 1;
}
