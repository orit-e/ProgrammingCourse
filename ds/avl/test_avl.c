#include "avl.h"
#include "tests.h"

int TestIntegraton1();
int TestIntegraton2();
int TestIntegraton3();
int TestIntegraton4();
int TestIntegraton5();
int TestAVLFind();
int TestAVLForeach();
int TestAVLBalancedInsert();
int TestAVLBalancedRemove();
int DataCMP(const void *data1, const void *data2, void *param);
int LetterUp(void *data, void *param);

typedef struct data_struct
{
	int num;
	char letter;
}data_struct_t;

avl_ty *avl = NULL;
data_struct_t a = {1, 'a'};
data_struct_t b = {2, 'b'};
data_struct_t c = {3, 'c'};
data_struct_t d = {4, 'd'};
data_struct_t e = {5, 'e'};
data_struct_t f = {6, 'f'};
data_struct_t g = {7, 'g'};
data_struct_t h = {8, 'h'};
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
	RunTest(TestIntegraton1(),			"TestIntegraton1");
	RunTest(TestIntegraton2(),			"TestIntegraton2");
	RunTest(TestIntegraton3(),			"TestIntegraton3");
	RunTest(TestIntegraton4(),			"TestIntegraton4");
	RunTest(TestIntegraton5(),			"TestIntegraton5");
	RunTest(TestAVLFind(),				"TestAVLFind");
	RunTest(TestAVLForeach(),			"TestAVLForeach");
	RunTest(TestAVLBalancedInsert(),	"TestAVLBalancedInsert");
	RunTest(TestAVLBalancedRemove(),	"TestAVLBalancedRemove");

	(void) argc;
	(void) argv;

    return 0;
}

int TestIntegraton1()
{
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (1 != AVLIsEmpty((const avl_ty *)avl));
	result += (0 != AVLSize((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}

int TestIntegraton2()
{
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLIsEmpty((const avl_ty *)avl));
	result += (1 != AVLSize((const avl_ty *)avl));
    result += (0 != AVLHeight((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}

int TestIntegraton3()
{
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &b));
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLInsert(avl, &c));
	result += (0 != AVLInsert(avl, &d));
	result += (0 != AVLIsEmpty((const avl_ty *)avl));
	result += (4 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	AVLRemove(avl, &d);
	result += (3 != AVLSize((const avl_ty *)avl));
    result += (1 != AVLHeight((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}

int TestIntegraton4()
{
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &b));
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLInsert(avl, &c));
	result += (0 != AVLInsert(avl, &d));
	result += (0 != AVLIsEmpty((const avl_ty *)avl));
	result += (4 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	AVLRemove(avl, &b);
	result += (3 != AVLSize((const avl_ty *)avl));
    result += (1 != AVLHeight((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}

int TestIntegraton5()
{
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &b));
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLInsert(avl, &d));
	result += (0 != AVLInsert(avl, &c));
	result += (0 != AVLInsert(avl, &e));
	result += (0 != AVLIsEmpty((const avl_ty *)avl));
	result += (5 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	AVLRemove(avl, &b);
	result += (4 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}

int TestAVLFind()
{	
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &d));
	result += (0 != AVLInsert(avl, &b));
	result += (0 != AVLInsert(avl, &f));
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLInsert(avl, &c));
	result += (0 != AVLInsert(avl, &e));
	result += (0 != AVLInsert(avl, &g));
	result += (0 != AVLIsEmpty((const avl_ty *)avl));
	result += (7 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	result += (&c != AVLFind((const avl_ty *)avl, &c));
	result += (&f != AVLFind((const avl_ty *)avl, &f));
	result += (NULL != AVLFind((const avl_ty *)avl, &bigger));
	result += (NULL != AVLFind((const avl_ty *)avl, &smaller));
	
	AVLDestroy(avl);
	
	return result;
}

int TestAVLForeach()
{	
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &d));
	result += (0 != AVLInsert(avl, &b));
	result += (0 != AVLInsert(avl, &f));
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLInsert(avl, &c));
	result += (0 != AVLInsert(avl, &e));
	result += (0 != AVLInsert(avl, &g));
	result += (0 != AVLIsEmpty((const avl_ty *)avl));
	result += (7 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLForeach(avl, &LetterUp, &add1, PREORDER));
	result += ('e' != d.letter);
	result += (0 != AVLForeach(avl, &LetterUp, &add1, INORDER));
	result += ('f' != d.letter);
	result += (0 != AVLForeach(avl, &LetterUp, &add1, POSTORDER));
	result += ('g' != d.letter);

	AVLDestroy(avl);
	
	return result;
}

int TestAVLBalancedInsert()
{	
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &a));
	result += (1 != AVLSize((const avl_ty *)avl));
    result += (0 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLInsert(avl, &b));
	result += (2 != AVLSize((const avl_ty *)avl));
    result += (1 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLInsert(avl, &c));
	result += (3 != AVLSize((const avl_ty *)avl));
    result += (1 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLInsert(avl, &d));
	result += (4 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLInsert(avl, &e));
	result += (5 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLInsert(avl, &f));
	result += (6 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	result += (0 != AVLInsert(avl, &g));
	result += (7 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}

int TestAVLBalancedRemove()
{	
	int result = 0;
	
	avl = AVLCreate(DataCMP, (void *)&param);
	
	result += (NULL == avl);
	result += (0 != AVLInsert(avl, &a));
	result += (0 != AVLInsert(avl, &b));
	result += (0 != AVLInsert(avl, &c));
	result += (0 != AVLInsert(avl, &d));
	result += (0 != AVLInsert(avl, &e));
	result += (0 != AVLInsert(avl, &f));
	result += (0 != AVLInsert(avl, &g));
	result += (0 != AVLInsert(avl, &h));
	AVLRemove(avl, &e);
	result += (7 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	AVLRemove(avl, &d);
	result += (6 != AVLSize((const avl_ty *)avl));
    result += (2 != AVLHeight((const avl_ty *)avl));
	AVLRemove(avl, &a);
	AVLRemove(avl, &g);
	AVLRemove(avl, &f);
	result += (3 != AVLSize((const avl_ty *)avl));
    result += (1 != AVLHeight((const avl_ty *)avl));
	
	AVLDestroy(avl);
	
	return result;
}