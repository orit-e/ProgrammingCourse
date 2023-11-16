#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "sorted_list.h"

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

int TestSortedListCreate();
int TestSortedListIsEmpty();
int TestSortedListBegin();
int TestSortedListEnd();
int TestSortedListNext();
int TestSortedListPrev();
int TestSortedListInsert();
int TestSortedListRemove();
int TestSortedListCount();
int TestSortedListPopFront();
int TestSortedListPopBack();
int TestSortedListIsEqual();
int TestSortedListGetData();
int TestSortedListFindIf();
int TestSortedListFind();
int TestSortedListForeach();
int TestSortedListMerge();
int TestSortedListDestroy();
int DataCMP(const void *data1, const void *data2);
int Add1(void *iter, void *param);
int Match(const void *iter, const void *param);

struct sorted_list
{
	dlist_t *dlist;
	sorted_list_cmp_func_t cmp;
};

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

sorted_list_t *sorted_list = NULL;
sorted_list_t *sorted_list2 = NULL;
int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;
int g = 0;


int DataCMP(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}

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
	RUN_TEST(TestSortedListCreate(),			"SortedListCreate");
	RUN_TEST(TestSortedListIsEmpty(),			"SortedListIsEmpty");
	RUN_TEST(TestSortedListBegin(),				"SortedListBegin");
	RUN_TEST(TestSortedListEnd(),				"SortedListEnd");
	RUN_TEST(TestSortedListNext(),				"SortedListNext");
	RUN_TEST(TestSortedListPrev(),				"SortedListPrev");
	RUN_TEST(TestSortedListInsert(),			"SortedListInsert");
	RUN_TEST(TestSortedListRemove(),			"SortedListRemove");
	RUN_TEST(TestSortedListCount(),				"SortedListCount");
	RUN_TEST(TestSortedListPopFront(),			"SortedListPopFront");
	RUN_TEST(TestSortedListPopBack(),			"SortedListPopBack");
	RUN_TEST(TestSortedListIsEqual(),			"SortedListIsEqual");
	RUN_TEST(TestSortedListGetData(),			"SortedListGetData");
	RUN_TEST(TestSortedListFindIf(),			"SortedListFindIf");
	RUN_TEST(TestSortedListFind(),				"SortedListFind");
	RUN_TEST(TestSortedListForeach(),			"SortedListForeach");
	RUN_TEST(TestSortedListMerge(),				"SortedListMerge");
	RUN_TEST(TestSortedListDestroy(),			"SortedListDestroy");

	(void) argc;
	(void) argv;

    return 0;
}

int TestSortedListCreate()
{
	sorted_list = SortedListCreate(DataCMP);
	return (NULL != sorted_list);
}

int TestSortedListIsEmpty()
{
	return (1 == SortedListIsEmpty(sorted_list));
}

int TestSortedListBegin()
{
	return (sorted_list->dlist->tail == SortedListBegin(sorted_list));
}

int TestSortedListEnd()
{
	return (sorted_list->dlist->tail == SortedListEnd(sorted_list));
}

int TestSortedListNext()
{	
	return (sorted_list->dlist->tail == SortedListNext(sorted_list->dlist->head));
}

int TestSortedListPrev()
{	
	return (sorted_list->dlist->head == SortedListPrev(sorted_list->dlist->tail));
}

int TestSortedListInsert()
{
	iterator_t iter = SortedListInsert(sorted_list, &c);
	int result = 0;
	
	result += (sorted_list->dlist->tail != iter);
	iter = SortedListInsert(sorted_list, &a);
	result += (sorted_list->dlist->tail != iter);
	iter = SortedListInsert(sorted_list, &b);
	result += (sorted_list->dlist->tail != iter);
	iter = SortedListInsert(sorted_list, &d);
	result += (sorted_list->dlist->tail != iter);
	
	iter = SortedListBegin(sorted_list);
	result += (&a == (int *)iter->data);
	iter = SortedListNext(iter);
	result += (&b == (int *)iter->data);
	iter = SortedListNext(iter);
	result += (&c == (int *)iter->data);
	iter = SortedListNext(iter);
	result += (&d == (int *)iter->data);
	
	return (8 == result);
}

int TestSortedListRemove()
{
	iterator_t iter = sorted_list->dlist->head->next->next;
	iterator_t iter_prev = sorted_list->dlist->head->next;
	int result = 0;
	
	iter = SortedListRemove(iter);
	result += (iter_prev->next == iter);
	result += (iter->prev == iter_prev);
	result += (c == *(int *)iter->data);
	
	return (3 == result);
}

int TestSortedListCount()
{
	return (3 == SortedListCount(sorted_list));
}

int TestSortedListPopFront()
{
	int *data = (int *)SortedListPopFront(sorted_list);
	iterator_t iter = sorted_list->dlist->head->next;
	int result = 0;
	
	result += (&a == data);
	result += (c == *(int *)iter->data);
	
	return (2 == result);
}

int TestSortedListPopBack()
{
	int *data = (int *)SortedListPopBack(sorted_list);
	iterator_t iter = sorted_list->dlist->head->next;
	int result = 0;
	
	result += (&d == data);
	result += (c == *(int *)iter->data);
	
	return (2 == result);
}

int TestSortedListIsEqual()
{
	iterator_t iter1 = sorted_list->dlist->head->next;
	iterator_t iter2 = SortedListInsert(sorted_list, &e);
	int result = 0;

	result += (0 == SortedListIsEqual(iter1, iter2));
	result += (1 == SortedListIsEqual(iter1->next, iter2));
	sorted_list2 = SortedListCreate(DataCMP);
	iter2 = SortedListInsert(sorted_list2, &c);
	result += (0 == SortedListIsEqual(iter1, iter2));
	
	return (3 == result);
}

int TestSortedListGetData()
{
	iterator_t iter = sorted_list->dlist->head->next;
	int result = 0;

	result += (c == *(int *) SortedListGetData(iter));
	result += (e == *(int *) SortedListGetData(iter->next));

	return (2 == result);	
}

int TestSortedListFindIf()
{
	iterator_t iter = SortedListFindIf(SortedListBegin(sorted_list), SortedListEnd(sorted_list), Match, (const void *)&c);
	int result = 0;
	
	result += (c == *(int *)SortedListGetData(iter));
	iter = SortedListFindIf(SortedListBegin(sorted_list), SortedListEnd(sorted_list), Match, (const void *)&f);
	result += (NULL == SortedListNext(iter));
	iter = SortedListFindIf(SortedListBegin(sorted_list), SortedListEnd(sorted_list), Match, (const void *)&d);
	result += (e == *(int *)SortedListGetData(iter));

	return (3 == result);
}

int TestSortedListFind()
{
	iterator_t iter = SortedListFind(SortedListBegin(sorted_list), SortedListEnd(sorted_list), (const void *)&c, sorted_list);
	int result = 0;
	
	result += (c == *(int *)SortedListGetData(iter));
	iter = SortedListFind(SortedListBegin(sorted_list), SortedListEnd(sorted_list), (const void *)&f, sorted_list);
	result += (NULL == SortedListNext(iter));
	iter = SortedListFind(SortedListBegin(sorted_list), SortedListEnd(sorted_list), (const void *)&d, sorted_list);
	result += (NULL == SortedListNext(iter));

	return (3 == result);
}

int TestSortedListForeach()
{
	iterator_t iter = SortedListBegin(sorted_list);
	int result = 0;

	result += (0 == SortedListForeach(SortedListBegin(sorted_list), SortedListEnd(sorted_list), Add1, (const void *)&a));
	result += (d == *(int *)SortedListGetData(iter));
	iter = SortedListNext(iter);
	result += (f == *(int *)SortedListGetData(iter));

	return (3 == result);
}

int TestSortedListMerge()
{
	iterator_t iter1 = SortedListBegin(sorted_list);
	iterator_t iter2 = SortedListBegin(sorted_list2);
	int result = 0;
	
	SortedListRemove(iter2);
	SortedListPopFront(sorted_list);
	SortedListPopFront(sorted_list);
	
	SortedListInsert(sorted_list, (const void *)&a);
	SortedListInsert(sorted_list, (const void *)&e);
	
	SortedListInsert(sorted_list2, (const void *)&b);
	SortedListInsert(sorted_list2, (const void *)&d);
	SortedListInsert(sorted_list2, (const void *)&f);
	
	SortedListMerge(sorted_list, sorted_list2);
	
	result += (5 == SortedListCount(sorted_list));
	result += (0 == SortedListCount(sorted_list2));
	
	iter1 = SortedListBegin(sorted_list);
	
	result += (a == *(int *)SortedListGetData(iter1));
	iter1 = SortedListNext(iter1);
	result += (b == *(int *)SortedListGetData(iter1));
	iter1 = SortedListNext(iter1);
	result += (d == *(int *)SortedListGetData(iter1));
	iter1 = SortedListNext(iter1);
	result += (e == *(int *)SortedListGetData(iter1));
	iter1 = SortedListNext(iter1);
	result += (f == *(int *)SortedListGetData(iter1));
	iter1 = SortedListNext(iter1);
	
	SortedListInsert(sorted_list2, (const void *)&g);
	SortedListMerge(sorted_list, sorted_list2);
	iter1 = SortedListBegin(sorted_list);
	result += (g == *(int *)SortedListGetData(iter1));

	return (8 == result);
}

int TestSortedListDestroy()
{
	SortedListDestroy(sorted_list);
	SortedListDestroy(sorted_list2);
	
	return DONE;
}
