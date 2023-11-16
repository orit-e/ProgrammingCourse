#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "dlist.h"

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

int TestDListCreate();
int TestDListIsEmpty();
int TestDListBegin();
int TestDListEnd();
int TestDListNext();
int TestDListPrev();
int TestDListInsertBefore();
int TestDListInsertAfter();
int TestDListRemove();
int TestDListCount();
int TestDListPushFront();
int TestDListPushBack();
int TestDListPopFront();
int TestDListPopBack();
int TestDListIsEqual();
int TestDListGetData();
int TestDListSetData();
int TestDListFind();
int TestDListForeach();
int TestDListSplice();
int TestDListMultiFind();
int TestDListDestroy();
int Add1(void *iter, void *param);
int Match(const void *iter, const void *param);

struct dlist 
{
	node_t *head;
	node_t *tail;	
};

struct node
{
	void *data;
	node_t *next;
	node_t *prev;
};

dlist_t *dlist = NULL;
dlist_t *dlist2 = NULL;
int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;

int Add1(void *data, void *param)
{
	if(*(int*) data >= *(int*) param)
	{
		*(int*) data = *(int*) data + 1;
	}

	return 0;
}

int Match(const void *data, const void *param)
{
	return(*(int*) data >= *(int*) param);
}

int main(int argc, char *argv[])
{
	RUN_TEST(TestDListCreate(),					"DListCreate");
	RUN_TEST(TestDListIsEmpty(),				"DListIsEmpty");
	RUN_TEST(TestDListBegin(),					"DListBegin");
	RUN_TEST(TestDListEnd(),					"DListEnd");
	RUN_TEST(TestDListNext(),					"DListNext");
	RUN_TEST(TestDListPrev(),					"DListPrev");
	RUN_TEST(TestDListInsertBefore(),			"DListInsertBefore");
	RUN_TEST(TestDListInsertAfter(),			"DListInsertAfter");
	RUN_TEST(TestDListRemove(),					"DListRemove");
	RUN_TEST(TestDListCount(),					"DListCount");
	RUN_TEST(TestDListPushFront(),				"DListPushFront");
	RUN_TEST(TestDListPushBack(),				"DListPushBack");
	RUN_TEST(TestDListPopFront(),				"DListPopFront");
	RUN_TEST(TestDListPopBack(),				"DListPopBack");
	RUN_TEST(TestDListIsEqual(),				"DListIsEqual");
	RUN_TEST(TestDListGetData(),				"DListGetData");
	RUN_TEST(TestDListSetData(),				"DListSetData");
	RUN_TEST(TestDListFind(),					"DListFind");
	RUN_TEST(TestDListForeach(),				"DListForeach");
	RUN_TEST(TestDListSplice(),					"DListSplice");
	RUN_TEST(TestDListMultiFind(),				"DListMultiFind");
	RUN_TEST(TestDListDestroy(),				"DListDestroy");

	(void) argc;
	(void) argv;

    return 0;
}

int TestDListCreate()
{
	dlist = DListCreate();
	return (NULL != dlist);
}

int TestDListIsEmpty()
{
	return (1 == DListIsEmpty(dlist));
}

int TestDListBegin()
{
	return (dlist->tail == DListBegin(dlist));
}

int TestDListEnd()
{
	return (dlist->tail == DListEnd(dlist));
}

int TestDListNext()
{	
	return (dlist->tail == DListNext(dlist->head));
}

int TestDListPrev()
{	
	return (dlist->head == DListPrev(dlist->tail));
}

int TestDListInsertBefore()
{
	iterator_t iter = dlist->tail;
	int result = 0;
	
	iter = DListInsertBefore(iter, &a);
	result += (dlist->tail == iter->next);
	result += (dlist->head == iter->prev);
	result += (a == *(int *)iter->data);
	result += (iter == dlist->tail->prev);
	result += (iter == dlist->head->next);
	
	return (5 == result);
}

int TestDListInsertAfter()
{
	iterator_t iter = dlist->head->next;
	int result = 0;
	
	iter = DListInsertAfter(iter, &b);
	result += (dlist->tail == iter->next);
	result += (dlist->head->next == iter->prev);
	result += (b == *(int *)iter->data);
	result += (iter == dlist->tail->prev);
	result += (iter == dlist->head->next->next);
	
	return (5 == result);
}

int TestDListRemove()
{
	iterator_t iter = dlist->head->next;
	int result = 0;
	
	iter = DListRemove(iter);
	result += (dlist->tail == iter->next);
	result += (dlist->head == iter->prev);
	result += (b == *(int *)iter->data);
	result += (iter == dlist->tail->prev);
	result += (iter == dlist->head->next);
	
	return (5 == result);
}

int TestDListCount()
{
	return (1 == DListCount(dlist));
}

int TestDListPushFront()
{
	iterator_t iter = DListPushFront(dlist, &a);
	int result = 0;
	
	result += (dlist->tail->prev == iter->next);
	result += (dlist->head == iter->prev);
	result += (a == *(int *)iter->data);
	result += (iter == dlist->tail->prev->prev);
	result += (iter == dlist->head->next);
	
	return (5 == result);
}

int TestDListPushBack()
{
	iterator_t iter = DListPushBack(dlist, &c);
	int result = 0;
	
	result += (dlist->tail == iter->next);
	result += (dlist->head->next->next == iter->prev);
	result += (c == *(int *)iter->data);
	result += (iter == dlist->tail->prev);
	result += (iter == dlist->head->next->next->next);
	
	return (5 == result);
}	

int TestDListPopFront()
{
	int *data = (int *)DListPopFront(dlist);
	iterator_t iter = dlist->head->next;
	int result = 0;
	
	result += (&a == data);
	result += (b == *(int *)iter->data);
	
	return (2 == result);
}

int TestDListPopBack()
{
	int *data = (int *)DListPopBack(dlist);
	iterator_t iter = dlist->tail->prev;
	int result = 0;
	
	result += (&c == data);
	result += (b == *(int *)iter->data);
	
	return (2 == result);
}

int TestDListIsEqual()
{
	iterator_t iter1 = dlist->head->next;
	iterator_t iter2 = DListInsertBefore(dlist->tail, &c);
	int result = 0;

	result += (0 == DListIsEqual(iter1, iter2));
	result += (1 == DListIsEqual(iter1->next, iter2));
	dlist2 = DListCreate();
	iter2 = DListPushFront(dlist2, &b);
	result += (0 == DListIsEqual(iter1, iter2));
	
	
	return (3 == result);
}

int TestDListGetData()
{
	iterator_t iter = DListBegin(dlist);
	int result = 0;

	result += (2 == *(int*) DListGetData(iter));
	iter = DListNext(iter);
	result += (3 == *(int*) DListGetData(iter));

	return (2 == result);	
}

int TestDListSetData()
{
	iterator_t iter = DListBegin(dlist);
	int result = 0;
	
	DListSetData(iter, &a);
	result += (1 == *(int *)iter->data);
	iter = DListNext(iter);
	DListSetData(iter, &b);
	result += (2 == *(int *)iter->data);
	
	return (2 == result);
}

int TestDListFind()
{
	iterator_t iter = DListFind(DListBegin(dlist), DListEnd(dlist), Match, (const void *)&b);
	int result = 0;
	
	result += (2 == *(int*) DListGetData(iter));
	iter = DListFind(DListBegin(dlist), DListEnd(dlist), Match, (const void *)&c);
	result += (NULL == DListNext(iter));

	return (2 == result);
}

int TestDListForeach()
{
	iterator_t iter = DListBegin(dlist);
	int result = 0;

	result += (0 == DListForeach(DListBegin(dlist), DListEnd(dlist), Add1, (const void *)&a));
	result += (2 == *(int*) DListGetData(iter));
	iter = DListNext(iter);
	result += (3 == *(int*) DListGetData(iter));

	return (3 == result);
}

int TestDListSplice()
{
	iterator_t iter1 = DListBegin(dlist);
	iterator_t iter2 = DListBegin(dlist2);
	int result = 0;
	
	DListPushFront(dlist, &a);
	DListSetData(iter2, &e);
	DListPushFront(dlist2, &d);
	
	iter1 = DListSplice(dlist->tail, DListBegin(dlist2), DListEnd(dlist2));
	
	result += (dlist->tail == iter1);
	result += (5 == DListCount(dlist));
	result += (0 == DListCount(dlist2));

	return (3 == result);
}

int TestDListMultiFind()
{
	iterator_t iter = DListBegin(dlist2);
	int result = 0;
	
	result += (0 == DListMultiFind(DListBegin(dlist), DListEnd(dlist), Match, (void const *)&c, dlist2));
	result += (3 == DListCount(dlist2));
	result += (5 == DListCount(dlist));
	iter = DListBegin(dlist2);
	result += (&b == (int *)DListGetData(iter));
	iter = DListNext(iter);
	result += (&d == (int *)DListGetData(iter));
	iter = DListNext(iter);
	result += (&e == (int *)DListGetData(iter));

	return (6 == result);
}

int TestDListDestroy()
{
	DListDestroy(dlist);
	DListDestroy(dlist2);
	
	return DONE;
}
