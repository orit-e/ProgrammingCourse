#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "queue.h"

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

int TestQCreate();
int TestQEnQueue();
int TestQPeek();
int TestQDeQueue();
int TestQueueIsEmpty();
int TestQSize();
int TestQAppend();
int TestQDestroy();

queue_t *queue = NULL;
queue_t *queue2 = NULL;
int d1 = 1;
int d2 = 2;
int d3 = 3;
int d4 = 4;
int d5 = 5;

int main(int argc, char *argv[])
{
	RUN_TEST(TestQCreate(),				"QCreate");
	RUN_TEST(TestQEnQueue(),			"QEnQueue");
	RUN_TEST(TestQPeek(),				"QPeek");
	RUN_TEST(TestQDeQueue(),			"QDeQueue");
	RUN_TEST(TestQueueIsEmpty(),		"QueueIsEmpty");
	RUN_TEST(TestQSize(),				"QSize");
	RUN_TEST(TestQAppend(),				"QAppend");
	RUN_TEST(TestQDestroy(),			"QDestroy");

	(void) argc;
	(void) argv;

    return 0;
}

int TestQCreate()
{
	queue = QCreate();
	return (NULL != queue);
}

int TestQEnQueue()
{
	int result = 0;
	result += QEnQueue(queue, &d1);
	result += QEnQueue(queue, &d2);
	result += QEnQueue(queue, &d3);
	return (0 == result);
}

int TestQPeek()
{
	return (1 == *(int*) QPeek(queue));	
}

int TestQDeQueue()
{
	QDeQueue(queue);
	
	return (2 == *(int*) QPeek(queue));
}

int TestQueueIsEmpty()
{
	int result = (1 != QueueIsEmpty(queue));
	
	QDeQueue(queue);
	QDeQueue(queue);
	result += (1 == QueueIsEmpty(queue));
	
	return (2 == result);
}

int TestQSize()
{
	int result = (0 == QSize(queue));
	
	QEnQueue(queue, &d1);
	QEnQueue(queue, &d2);
	QEnQueue(queue, &d3);
	result += (3 == QSize(queue));
	QDeQueue(queue);
	result += (2 == QSize(queue));
	
	return (3 == result);
}

int TestQAppend()
{
	int result = 0;
	
	queue2 = QCreate();
	QEnQueue(queue2, &d1);
	QEnQueue(queue2, &d4);
	QEnQueue(queue2, &d5);
	QAppend(queue,queue2);
	result += (1 == QueueIsEmpty(queue2));
	result += (5 == QSize(queue));
	
	return (2 == result);
}

int TestQDestroy()
{
	QDestroy(queue);
	QDestroy(queue2);
	return DONE;
}
