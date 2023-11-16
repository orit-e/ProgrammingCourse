#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "bst.h"

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

int TestIntegraton1();
int TestIntegraton2();
int TestIntegraton3();
int TestIntegraton4();
int TestIntegraton5();
int TestBSTFind();
int TestBSTForeach();
int DataCMP(const void *data1, const void *data2, void *param);
int LetterUp(void *data, void *param);

typedef struct data_struct
{
	int num;
	char letter;
}data_struct_t;

bst_ty *bst = NULL;
data_struct_t a = {1, 'a'};
data_struct_t b = {2, 'b'};
data_struct_t c = {3, 'c'};
data_struct_t d = {4, 'd'};
data_struct_t e = {5, 'e'};
data_struct_t f = {6, 'f'};
data_struct_t g = {7, 'g'};
data_struct_t bigger = {10, 'i'};
data_struct_t smaller = {0, 'I'};
int param = 0;
int add1 = 1;

int DataCMP(const void *data1, const void *data2, void *param)
{
	data_struct_t *struct1 = (data_struct_t *)data1;
	data_struct_t *struct2 = (data_struct_t *)data2;
	
	return (struct1->num - struct2->num + *(int *)param);
}

int LetterUp(void *data, void *param)
{
	data_struct_t *my_struct = (data_struct_t *)data;
	
	my_struct->letter += *(int *)param;

	return 0;
}

int Match(const void *data, const void *param)
{
	return(*(int*) data >= *(int*) param);
}

int main(int argc, char *argv[])
{
	RUN_TEST(TestIntegraton1(),			"TestIntegraton1");
	RUN_TEST(TestIntegraton2(),			"TestIntegraton2");
	RUN_TEST(TestIntegraton3(),			"TestIntegraton3");
	RUN_TEST(TestIntegraton4(),			"TestIntegraton4");
	RUN_TEST(TestIntegraton5(),			"TestIntegraton5");
	RUN_TEST(TestBSTFind(),				"TestBSTFind");
	RUN_TEST(TestBSTForeach(),			"TestBSTForeach");

	(void) argc;
	(void) argv;

    return 0;
}

int TestIntegraton1()
{
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	
	result += (NULL != bst);
	result += (1 == BSTIsEmpty((const bst_ty *)bst));
	result += (0 == BSTSize((const bst_ty *)bst));
	result += (BSTBegin(bst) == BSTEnd(bst));
	
	BSTDestroy(bst);
	
	return (4 == result);
}

int TestIntegraton2()
{
	iter_ty iter = NULL;
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	
	result += (NULL != bst);
	iter = BSTInsert(bst, (const void *)&a);
	result += (NULL != iter);
	result += (0 == BSTIsEmpty((const bst_ty *)bst));
	result += (1 == BSTSize((const bst_ty *)bst));
	result += (&a == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&b);
	result += (NULL != iter);
	result += (2 == BSTSize((const bst_ty *)bst));
	result += (&b == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&c);
	result += (NULL != iter);
	result += (3 == BSTSize((const bst_ty *)bst));
	result += (&c == BSTGetData(iter));
	
	iter = BSTBegin(bst);
	result += (&a == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&b == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&c == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (BSTEnd(bst) == iter);
	
	iter = BSTPrev(iter);
	result += (&c == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&b == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&a == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (BSTEnd(bst) == iter);
	
	BSTDestroy(bst);
	
	return (19 == result);
}

int TestIntegraton3()
{
	iter_ty iter = NULL;
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	
	result += (NULL != bst);
	iter = BSTInsert(bst, (const void *)&d);
	result += (NULL != iter);
	result += (0 == BSTIsEmpty((const bst_ty *)bst));
	result += (1 == BSTSize((const bst_ty *)bst));
	result += (&d == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&b);
	result += (NULL != iter);
	result += (2 == BSTSize((const bst_ty *)bst));
	result += (&b == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&f);
	result += (NULL != iter);
	result += (3 == BSTSize((const bst_ty *)bst));
	result += (&f == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&a);
	result += (NULL != iter);
	result += (4 == BSTSize((const bst_ty *)bst));
	result += (&a == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&c);
	result += (NULL != iter);
	result += (5 == BSTSize((const bst_ty *)bst));
	result += (&c == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&e);
	result += (NULL != iter);
	result += (6 == BSTSize((const bst_ty *)bst));
	result += (&e == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&g);
	result += (NULL != iter);
	result += (7 == BSTSize((const bst_ty *)bst));
	result += (&g == BSTGetData(iter));
	
	iter = BSTBegin(bst);
	result += (&a == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&b == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&c == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&d == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&e == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&f == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (&g == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (BSTEnd(bst) == iter);
	
	
	iter = BSTPrev(iter);
	result += (&g == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&f == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&e == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&d == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&c == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&b == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&a == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (BSTEnd(bst) == iter);
	
	BSTDestroy(bst);
	
	return (39 == result);
}

int TestIntegraton4()
{
	iter_ty iter = NULL;
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	
	result += (NULL != bst);
	iter = BSTInsert(bst, (const void *)&c);
	result += (NULL != iter);
	result += (0 == BSTIsEmpty((const bst_ty *)bst));
	result += (1 == BSTSize((const bst_ty *)bst));
	result += (&c == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&b);
	result += (NULL != iter);
	result += (2 == BSTSize((const bst_ty *)bst));
	result += (&b == BSTGetData(iter));
	iter = BSTInsert(bst, (const void *)&a);
	result += (NULL != iter);
	result += (3 == BSTSize((const bst_ty *)bst));
	result += (&a == BSTGetData(iter));
	
	iter = BSTBegin(bst);
	result += (&a == BSTGetData(iter));
	result += (1 == BSTIterIsEqual(iter, BSTBegin(bst)));
	iter = BSTNext(iter);
	result += (&b == BSTGetData(iter));
	result += (0 == BSTIterIsEqual(iter, BSTBegin(bst)));
	iter = BSTNext(iter);
	result += (&c == BSTGetData(iter));
	iter = BSTNext(iter);
	result += (BSTEnd(bst) == iter);
	
	iter = BSTPrev(iter);
	result += (&c == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&b == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (&a == BSTGetData(iter));
	iter = BSTPrev(iter);
	result += (BSTEnd(bst) == iter);
	
	BSTDestroy(bst);
	
	return (21 == result);
}

int TestIntegraton5()
{
	iter_ty iter = NULL;
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&c);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&g);
	iter = BSTBegin(bst);
	BSTRemove(iter);
	result += (6 == BSTSize((const bst_ty *)bst));
	iter = BSTBegin(bst);
	BSTRemove(iter);
	result += (5 == BSTSize((const bst_ty *)bst));
	BSTDestroy(bst);
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&c);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&g);
	iter = BSTBegin(bst);
	iter = BSTNext(iter);
	BSTRemove(iter);
	result += (6 == BSTSize((const bst_ty *)bst));
	BSTDestroy(bst);
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&g);
	iter = BSTBegin(bst);
	iter = BSTNext(iter);
	BSTRemove(iter);
	result += (5 == BSTSize((const bst_ty *)bst));
	BSTDestroy(bst);
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&c);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&g);
	iter = BSTBegin(bst);
	iter = BSTNext(iter);
	iter = BSTNext(iter);
	iter = BSTNext(iter);
	BSTRemove(iter);
	result += (6 == BSTSize((const bst_ty *)bst));
	BSTDestroy(bst);
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&c);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTBegin(bst);
	iter = BSTNext(iter);
	BSTRemove(iter);
	result += (5 == BSTSize((const bst_ty *)bst));
	BSTDestroy(bst);
	
	return (6 == result);
}

int TestBSTFind()
{	
	iter_ty iter = NULL;
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&c);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&g);
	
	iter = BSTFind(bst, (const void *)&a);
	result += (&a == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&b);
	result += (&b == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&c);
	result += (&c == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&d);
	result += (&d == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&e);
	result += (&e == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&f);
	result += (&f == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&g);
	result += (&g == BSTGetData(iter));
	iter = BSTFind(bst, (const void *)&bigger);
	result += (BSTEnd(bst) == iter);
	iter = BSTFind(bst, (const void *)&smaller);
	result += (BSTEnd(bst) == iter);
	
	BSTDestroy(bst);
	
	return (9 == result);
}

int TestBSTForeach()
{	
	iter_ty iter = NULL;
	int result = 0;
	
	bst = BSTCreate(DataCMP, (void *)&param);
	iter = BSTInsert(bst, (const void *)&d);
	iter = BSTInsert(bst, (const void *)&b);
	iter = BSTInsert(bst, (const void *)&f);
	iter = BSTInsert(bst, (const void *)&a);
	iter = BSTInsert(bst, (const void *)&c);
	iter = BSTInsert(bst, (const void *)&e);
	iter = BSTInsert(bst, (const void *)&g);
	
	BSTForeach(BSTBegin(bst), BSTEnd(bst), LetterUp, (void *)&add1);
	
	iter = BSTBegin(bst);
	result += ('b' == ((data_struct_t *)BSTGetData(iter))->letter);
	iter = BSTNext(iter);
	result += ('c' == ((data_struct_t *)BSTGetData(iter))->letter);
	iter = BSTNext(iter);
	result += ('d' == ((data_struct_t *)BSTGetData(iter))->letter);
	iter = BSTNext(iter);
	result += ('e' == ((data_struct_t *)BSTGetData(iter))->letter);
	iter = BSTNext(iter);
	result += ('f' == ((data_struct_t *)BSTGetData(iter))->letter);
	iter = BSTNext(iter);
	result += ('g' == ((data_struct_t *)BSTGetData(iter))->letter);
	iter = BSTNext(iter);
	result += ('h' == ((data_struct_t *)BSTGetData(iter))->letter);
	
	BSTDestroy(bst);
	
	return (7 == result);
}
