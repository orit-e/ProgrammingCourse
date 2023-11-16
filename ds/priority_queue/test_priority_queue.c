#include <stdio.h> /* printf */

#include "priority_queue.h"
#include "tests.h"

int TestIntegraton1();
int TestIntegraton2();
int TestIntegraton3();
int TestIntegraton4();
int TestIntegraton5();
int TestIntegraton6();
int TestIntegraton7();
int PDataCMP(const void *data1, const void *data2, void *param);
int Match(const void *iter, const void *param);

typedef struct data_struct
{
	int num;
	char letter;
}data_struct_t;

priority_q_t *pq = NULL;
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

int PDataCMP(const void *data1, const void *data2, void *param)
{
	data_struct_t *struct1 = (data_struct_t *)data1;
	data_struct_t *struct2 = (data_struct_t *)data2;
	
	return (struct1->num - struct2->num + *(int *)param);
}

int Match(const void *data, const void *param)
{
	data_struct_t *my_struct = (data_struct_t *)data;

	return (my_struct->letter == *(char *)param);
}

int main(int argc, char *argv[])
{
	RunTest(TestIntegraton1(),		"TestIntegraton1");
	RunTest(TestIntegraton2(),		"TestIntegraton2");
	RunTest(TestIntegraton3(),		"TestIntegraton3");
	RunTest(TestIntegraton4(),		"TestIntegraton4");
	RunTest(TestIntegraton5(),		"TestIntegraton5");
	RunTest(TestIntegraton6(),		"TestIntegraton6");
	RunTest(TestIntegraton7(),		"TestIntegraton7");

	(void) argc;
	(void) argv;

    return 0;
}

int TestIntegraton1()
{
	int result = 0;
	
	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (1 != PQueueIsEmpty((const priority_q_t *)pq));
	result += (0 != PQSize((const priority_q_t *)pq));
	
	PQDestroy(pq);
	
	return result;
}

int TestIntegraton2()
{
	int result = 0;
	
	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (0 != PQEnQueue(pq, &a));
	result += (0 != PQueueIsEmpty((const priority_q_t *)pq));
	result += (1 != PQSize((const priority_q_t *)pq));
    result += (&a != PQPeek((const priority_q_t *)pq));
	
	PQDestroy(pq);
	
	return result;
}

int TestIntegraton3()
{
	int result = 0;
	
	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (0 != PQEnQueue(pq, &b));
	result += (&b != PQPeek((const priority_q_t *)pq));
	result += (0 != PQEnQueue(pq, &a));
	result += (&b != PQPeek((const priority_q_t *)pq));
	result += (0 != PQEnQueue(pq, &c));
	result += (&c != PQPeek((const priority_q_t *)pq));
	result += (0 != PQEnQueue(pq, &d));
	result += (&d != PQPeek((const priority_q_t *)pq));
	result += (0 != PQueueIsEmpty((const priority_q_t *)pq));
	result += (4 != PQSize((const priority_q_t *)pq));
	
	PQDestroy(pq);
	
	return result;
}

int TestIntegraton4()
{
	int result = 0;
	
	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (0 != PQEnQueue(pq, &f));
	result += (0 != PQEnQueue(pq, &c));
	result += (0 != PQEnQueue(pq, &d));
	result += (0 != PQEnQueue(pq, &a));
	result += (0 != PQueueIsEmpty((const priority_q_t *)pq));
	result += (4 != PQSize((const priority_q_t *)pq));
	result += (&f != PQDeQueue(pq));
	result += (&d != PQPeek(pq));
	result += (3 != PQSize((const priority_q_t *)pq));
	
	PQDestroy(pq);
	
	return result;
}

int TestIntegraton5()
{
	int result = 0;

	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (0 != PQEnQueue(pq, &f));
	result += (0 != PQEnQueue(pq, &c));
	result += (0 != PQEnQueue(pq, &d));
	result += (0 != PQEnQueue(pq, &a));
	result += (0 != PQEnQueue(pq, &g));
	result += (0 != PQueueIsEmpty((const priority_q_t *)pq));
	result += (5 != PQSize((const priority_q_t *)pq));
	result += (&g != PQDeQueue(pq));
	result += (&f != PQPeek(pq));
	result += (4 != PQSize((const priority_q_t *)pq));
	result += (&f != PQDeQueue(pq));
	result += (&d != PQPeek(pq));
	result += (3 != PQSize((const priority_q_t *)pq));
	
	PQDestroy(pq);
	
	return result;
}

int TestIntegraton6()
{
	int result = 0;

	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (0 != PQEnQueue(pq, &g));
	result += (0 != PQEnQueue(pq, &d));
	result += (0 != PQEnQueue(pq, &f));
	result += (0 != PQEnQueue(pq, &c));
	result += (0 != PQEnQueue(pq, &a));
	result += (0 != PQEnQueue(pq, &b));
	result += (0 != PQEnQueue(pq, &e));
	result += (7 != PQSize((const priority_q_t *)pq));
	result += (&g != PQPeek((const priority_q_t *)pq));
	match_param = 'a';
	result += (&a != PQErase(pq, &Match, (void *)&match_param));
	result += (&g != PQPeek((const priority_q_t *)pq));
	result += (6 != PQSize((const priority_q_t *)pq));
	match_param = 'g';
	result += (&g != PQErase(pq, &Match, (void *)&match_param));
	match_param = 'h';
	result += (NULL != PQErase(pq, &Match, (void *)&match_param));
	result += (&f != PQPeek((const priority_q_t *)pq));
	result += (5 != PQSize((const priority_q_t *)pq));
	result += (&f != PQDeQueue(pq));
	result += (&e != PQPeek((const priority_q_t *)pq));

	PQDestroy(pq);
	
	return result;
}

int TestIntegraton7()
{
	int result = 0;

	pq = PQCreate(&PDataCMP, (void *)&param);
	
	result += (NULL == pq);
	result += (0 != PQEnQueue(pq, &a));
	result += (0 != PQEnQueue(pq, &b));
	result += (0 != PQEnQueue(pq, &c));
	result += (0 != PQEnQueue(pq, &d));
	result += (0 != PQEnQueue(pq, &d1));
	result += (0 != PQEnQueue(pq, &e));
	result += (0 != PQEnQueue(pq, &f));
	result += (0 != PQEnQueue(pq, &f1));
	result += (&f != PQPeek((const priority_q_t *)pq));
	result += (&f != PQDeQueue(pq));
	result += (&f1 != PQPeek((const priority_q_t *)pq));
	result += (&f1 != PQDeQueue(pq));
	result += (&e != PQPeek((const priority_q_t *)pq));
	result += (&e != PQDeQueue(pq));
	result += (&d != PQPeek((const priority_q_t *)pq));
	PQClear(pq);
	result += (1 != PQueueIsEmpty((const priority_q_t *)pq));
	result += (0 != PQSize((const priority_q_t *)pq));

	PQDestroy(pq);
	
	return result;
}