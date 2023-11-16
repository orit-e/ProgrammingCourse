#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "slist.h"

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

int TestSListCreate();
int TestIsEmptyList();
int TestSListInsert();
int TestSListRemove();
int TestSListGetData();
int TestSListSetData();
int TestSListFindInSList();
int TestSListCount();
int TestSListForeach();
int TestSListEnd();
int TestSListBegin();
int TestSListNext();
int TestSListIsEqual();
int TestSListDestroy();
int Add1(void *iter, void *param);
int Match(const void *iter, void *param);

struct slist 
{
	node_t *head;
	node_t *tail;
	
};

struct node
{
	void *value;
	node_t *next;
};

slist_t *slist = NULL;
iterator_t iter = NULL;
int d1 = 1;
int d2 = 2;
int d3 = 3;
int param1 = 2;
int param2 = 4;

int Add1(void *data, void *param)
{
	if(*(int*) data >= *(int*) param)
	{
		*(int*) data = *(int*) data + 1;
	}

	return 1;
}

int Match(const void *data, void *param)
{
	return(*(int*) data >= *(int*) param);
}

int main(int argc, char *argv[])
{
	RUN_TEST(TestSListCreate(),			"SListCreate");
	RUN_TEST(TestIsEmptyList(),			"SListIsEmptyList");
	RUN_TEST(TestSListBegin(),			"SListBegin");
	RUN_TEST(TestSListInsert(),			"SListInsert");
	RUN_TEST(TestSListGetData(),		"SListGetData");
	RUN_TEST(TestSListRemove(),			"SListRemove");
	RUN_TEST(TestSListSetData(),		"SListSetData");
	RUN_TEST(TestSListFindInSList(),	"SListFindInSList");
	RUN_TEST(TestSListCount(),			"SListCount");
	RUN_TEST(TestSListForeach(),		"SListForeach");
	RUN_TEST(TestSListEnd(),			"SListEnd");
	RUN_TEST(TestSListNext(),			"SListNext");
	RUN_TEST(TestSListIsEqual(),		"SListIsEqual");
	RUN_TEST(TestSListDestroy(),		"SListDestroy");

	(void) argc;
	(void) argv;

    return 0;
}

int TestSListCreate()
{
	slist = SListCreate();
	return DONE;
}

int TestIsEmptyList()
{
	return (1 == SListIsEmptyList(slist));
}

int TestSListBegin()
{
	iter = SListBegin(slist);
	return DONE;
}

int TestSListInsert()
{
	SListInsert(iter, &d1);
	iter = SListNext(iter);
	SListInsert(iter, &d2);
	iter = SListNext(iter);
	SListInsert(iter, &d3);
	return DONE;
}

int TestSListGetData()
{
	int result = 0;
	iter = SListBegin(slist);

	result += (1 == *(int*) SListGetData(iter));
	iter = SListNext(iter);
	result += (2 == *(int*) SListGetData(iter));
	iter = SListNext(iter);
	result += (3 == *(int*) SListGetData(iter));

	return (3 == result);	
}

int TestSListRemove()
{
	iter = SListBegin(slist);
	iter = SListNext(iter);
	iter = SListRemove(iter);
	return (3 == *(int*) SListGetData(iter));
}

int TestSListSetData()
{
	iter = SListBegin(slist);
	iter = SListNext(iter);
	SListSetData(iter, &d2);
	return DONE;
}

int TestSListFindInSList()
{
	int result = 0;
	iter = SListFindInSlist(SListBegin(slist), SListEnd(slist), Match, &param1);
	result += (2 == *(int*) SListGetData(iter));
	iter = SListFindInSlist(SListBegin(slist), SListEnd(slist), Match, &param2);
	result += (NULL != SListNext(iter));

	return (1 == result);
}

int TestSListCount()
{
	return (2 == SListCount(slist));
}

int TestSListForeach()
{
	int result = 0;

	SListForeach(SListBegin(slist), SListEnd(slist), Add1, &param1);
	iter = SListBegin(slist);
	result += (1 == *(int*) SListGetData(iter));
	iter = SListNext(iter);
	result += (3 == *(int*) SListGetData(iter));

	return (2 == result);
}

int TestSListEnd()
{
	iter = SListEnd(slist);
	return (NULL == SListNext(iter));
}

int TestSListNext()
{
	iter = SListBegin(slist);
	iter = SListNext(iter);
	
	return (3 == *(int*) SListGetData(iter));
}

int TestSListIsEqual()
{
	int result = 0;
	iterator_t iter2 = SListFindInSlist(SListBegin(slist), SListEnd(slist),
							Match, (void*)&d2);
	result += SListIsEqual(iter, SListBegin(slist));
	result += SListIsEqual(iter, iter2);
	
	return (1 == result);
}

int TestSListDestroy()
{
	SListDestroy(slist);
	return DONE;
}
