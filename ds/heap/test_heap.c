#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "heap.h"
#include "tests.h"

int TestIntegraton1();
int TestIntegraton2();
int TestIntegraton3();
int TestIntegraton4();
int TestIntegraton5();
int TestIntegraton6();

int DataCMP(const void *data1, const void *data2, void *param);
int IsMatch(void *data, void *param);

typedef struct data_struct
{
	int num;
	char letter;
}data_struct_t;

heap_ty *heap = NULL;
data_struct_t a = {1, 'a'};
data_struct_t b = {2, 'b'};
data_struct_t c = {3, 'c'};
data_struct_t d = {4, 'd'};
data_struct_t d1 = {4, 'd'};
data_struct_t e = {5, 'e'};
data_struct_t f = {6, 'f'};
data_struct_t f1 = {6, 'f'};
data_struct_t g = {7, 'g'};
data_struct_t bigger = {10, 'i'};
data_struct_t smaller = {0, 'I'};
int param = 0;
char match_param = 0;

int DataCMP(const void *data1, const void *data2, void *param)
{
	data_struct_t *struct1 = (data_struct_t *)data1;
	data_struct_t *struct2 = (data_struct_t *)data2;
	
	return (struct1->num - struct2->num + *(int *)param);
}

int IsMatch(void *data, void *match_param)
{
	data_struct_t *my_struct = (data_struct_t *)data;

	return (my_struct->letter == *(char *)match_param);
}

int main(int argc, char *argv[])
{
	RunTest(TestIntegraton1(),			"TestIntegraton1");
	RunTest(TestIntegraton2(),			"TestIntegraton2");
	RunTest(TestIntegraton3(),			"TestIntegraton3");
	RunTest(TestIntegraton4(),			"TestIntegraton4");
	RunTest(TestIntegraton5(),			"TestIntegraton5");
	RunTest(TestIntegraton6(),			"TestIntegraton6");

	(void)argc;
	(void)argv;

    return 0;
}

int TestIntegraton1()
{
	int result = 0;
	
	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (1 != HeapIsEmpty((const heap_ty *)heap));
	result += (0 != HeapSize((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton2()
{
	int result = 0;
	
	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapIsEmpty((const heap_ty *)heap));
	result += (1 != HeapSize((const heap_ty *)heap));
    result += (&a != HeapPeek((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton3()
{
	int result = 0;
	
	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &b));
	result += (&b != HeapPeek((const heap_ty *)heap));
	result += (0 != HeapPush(heap, &a));
	result += (&b != HeapPeek((const heap_ty *)heap));
	result += (0 != HeapPush(heap, &c));
	result += (&c != HeapPeek((const heap_ty *)heap));
	result += (0 != HeapPush(heap, &d));
	result += (&d != HeapPeek((const heap_ty *)heap));
	result += (0 != HeapIsEmpty((const heap_ty *)heap));
	result += (4 != HeapSize((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton4()
{
	int result = 0;
	
	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &f));
	result += (0 != HeapPush(heap, &c));
	result += (0 != HeapPush(heap, &d));
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapIsEmpty((const heap_ty *)heap));
	result += (4 != HeapSize((const heap_ty *)heap));
	HeapPop(heap);
	result += (&d != HeapPeek((const heap_ty *)heap));
	result += (3 != HeapSize((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton5()
{
	int result = 0;

	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &f));
	result += (0 != HeapPush(heap, &c));
	result += (0 != HeapPush(heap, &d));
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapPush(heap, &g));
	result += (0 != HeapIsEmpty((const heap_ty *)heap));
	result += (5 != HeapSize((const heap_ty *)heap));
	result += (&g != HeapPeek((const heap_ty *)heap));
	HeapPop(heap);
	result += (&f != HeapPeek((const heap_ty *)heap));
	result += (4 != HeapSize((const heap_ty *)heap));
	HeapPop(heap);
	result += (&d != HeapPeek((const heap_ty *)heap));
	result += (3 != HeapSize((const heap_ty *)heap));
	
	HeapDestroy(heap);
	
	return result;
}

int TestIntegraton6()
{
	int result = 0;

	heap = HeapCreate(DataCMP, (void *)&param);
	
	result += (NULL == heap);
	result += (0 != HeapPush(heap, &g));
	result += (0 != HeapPush(heap, &d));
	result += (0 != HeapPush(heap, &f));
	result += (0 != HeapPush(heap, &c));
	result += (0 != HeapPush(heap, &a));
	result += (0 != HeapPush(heap, &b));
	result += (0 != HeapPush(heap, &e));
	result += (7 != HeapSize((const heap_ty *)heap));
	result += (&g != HeapPeek((const heap_ty *)heap));
	match_param = 'a';
	result += (&a != HeapRemove(heap, &IsMatch, (void *)&match_param));
	result += (&g != HeapPeek((const heap_ty *)heap));
	result += (6 != HeapSize((const heap_ty *)heap));
	match_param = 'g';
	result += (&g != HeapRemove(heap, &IsMatch, (void *)&match_param));
	match_param = 'h';
	result += (NULL != HeapRemove(heap, &IsMatch, (void *)&match_param));
	result += (&f != HeapPeek((const heap_ty *)heap));
	result += (5 != HeapSize((const heap_ty *)heap));
	HeapPop(heap);
	result += (&e != HeapPeek((const heap_ty *)heap));

	HeapDestroy(heap);
	
	return result;
}