#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "hash.h"
#include "spell_checker.h"
#include "tests.h"

int TestIntegraton1();
int TestIntegraton2();
int TestIntegraton3();
int TestIntegraton4();
int TestIntegraton5();
int TestHashLoadAndHashStdev();
int TestSpellChecker();

int CMP(const void *data1, const void *data2);
size_t KeyToIndex(const void *data);
int LetterUp(void *data, void *op_param);

typedef struct data_struct
{
	int num;
	char letter;
}data_struct_t;

hash_ty *hash = NULL;
data_struct_t a = {1, 'a'};
data_struct_t b = {2, 'b'};
data_struct_t c = {3, 'c'};
data_struct_t d = {4, 'd'};
data_struct_t d1 = {12, 'd'};
data_struct_t e = {5, 'e'};
data_struct_t f = {6, 'f'};
data_struct_t f1 = {14, 'f'};
data_struct_t g = {7, 'g'};
data_struct_t bigger = {10, 'i'};
data_struct_t smaller = {0, 'I'};
int op_param = 1;

int CMP(const void *data1, const void *data2)
{
	data_struct_t *struct1 = (data_struct_t *)data1;
	data_struct_t *struct2 = (data_struct_t *)data2;
	
	return (struct1->num - struct2->num);
}

size_t KeyToIndex(const void *data)
{
    data_struct_t *my_struct = (data_struct_t *)data;

    return (my_struct->num % 8);
}

int LetterUp(void *data, void *op_param)
{
	data_struct_t *my_struct = (data_struct_t *)data;

	my_struct->letter += *(int *)op_param;
	return 0;
}

int main(int argc, char *argv[])
{
	RunTest(TestIntegraton1(),				"TestIntegraton1");
	RunTest(TestIntegraton2(),				"TestIntegraton2");
	RunTest(TestIntegraton3(),				"TestIntegraton3");
	RunTest(TestIntegraton4(),				"TestIntegraton4");
	RunTest(TestIntegraton5(),				"TestIntegraton5");
	RunTest(TestHashLoadAndHashStdev(),		"TestHashLoadAndHashStdev");
	RunTest(TestSpellChecker(),				"TestSpellChecker");

	(void) argc;
	(void) argv;

    return 0;
}

int TestIntegraton1()
{
	int result = 0;
	
	hash = HashCreate(KeyToIndex, 8, CMP);
	
	result += (NULL == hash);
	result += (1 != HashIsEmpty((const hash_ty *)hash));
	result += (0 != HashSize((const hash_ty *)hash));
	
	HashDestroy(hash);
	
	return result;
}

int TestIntegraton2()
{
	int result = 0;
	
	hash = HashCreate(KeyToIndex, 8, CMP);
	
	result += (NULL == hash);
	result += (0 != HashInsert(hash, &a));
	result += (0 != HashIsEmpty((const hash_ty *)hash));
	result += (1 != HashSize((const hash_ty *)hash));
	
	HashDestroy(hash);
	
	return result;
}

int TestIntegraton3()
{
	int result = 0;
	
	hash = HashCreate(KeyToIndex, 8, CMP);
	
	result += (NULL == hash);
	result += (0 != HashInsert(hash, &b));
	result += (0 != HashInsert(hash, &d));
	result += (0 != HashInsert(hash, &a));
	result += (0 != HashInsert(hash, &c));
	result += (0 != HashInsert(hash, &d1));
	result += (0 != HashIsEmpty((const hash_ty *)hash));
	result += (5 != HashSize((const hash_ty *)hash));
	HashRemove(hash, &d);
	result += (4 != HashSize((const hash_ty *)hash));
	HashRemove(hash, &e);
	result += (4 != HashSize((const hash_ty *)hash));

	HashDestroy(hash);
	
	return result;
}

int TestIntegraton4()
{
	int result = 0;
	
	hash = HashCreate(KeyToIndex, 8, CMP);
	
	result += (NULL == hash);
	result += (0 != HashInsert(hash, &b));
	result += (0 != HashInsert(hash, &d));
	result += (0 != HashInsert(hash, &a));
	result += (0 != HashInsert(hash, &c));
	result += (0 != HashInsert(hash, &d1));
	result += (0 != HashIsEmpty((const hash_ty *)hash));
	result += (5 != HashSize((const hash_ty *)hash));
	result += (&d != HashFind(hash, &d));
	result += (5 != HashSize((const hash_ty *)hash));
	HashRemove(hash, &d);
	result += (NULL != HashFind(hash, &d));
	result += (4 != HashSize((const hash_ty *)hash));
	result += (NULL != HashFind(hash, &e));
	result += (4 != HashSize((const hash_ty *)hash));

	HashDestroy(hash);
	
	return result;
}

int TestIntegraton5()
{
	int result = 0;
	
	hash = HashCreate(KeyToIndex, 8, CMP);
	
	result += (NULL == hash);
	result += (0 != HashInsert(hash, &b));
	result += (0 != HashInsert(hash, &d));
	result += (0 != HashInsert(hash, &a));
	result += (0 != HashInsert(hash, &c));
	result += (0 != HashInsert(hash, &d1));
	result += (0 != HashForEach(hash, LetterUp, &op_param));
	result += ('b' != a.letter);
	result += ('e' != d1.letter);
	result += (5 != HashSize((const hash_ty *)hash));

	HashDestroy(hash);
	
	return result;
}

int TestHashLoadAndHashStdev()
{
	int result = 0;
	double stdev = 0;
	
	hash = HashCreate(KeyToIndex, 8, CMP);
	
	result += (NULL == hash);
	result += (0 != HashInsert(hash, &b));
	result += (0 != HashInsert(hash, &d));
	result += (0 != HashInsert(hash, &a));
	result += (0 != HashInsert(hash, &c));
	result += (0 != HashInsert(hash, &f));
	result += (0 != HashInsert(hash, &e));
	result += (0 != HashInsert(hash, &g));
	result += (0 != HashInsert(hash, &smaller));
	result += (8 != HashSize((const hash_ty *)hash));
	result += (1 != HashLoad((const hash_ty *)hash));
	result += (0 != HashStdev((const hash_ty *)hash));
	result += (0 != HashInsert(hash, &d1));
	result += (0 != HashInsert(hash, &f1));
	result += (1.25 != HashLoad((const hash_ty *)hash));
	stdev = HashStdev((const hash_ty *)hash);
	result += (0.00000000001 > 0.4629100499 - stdev);

	HashDestroy(hash);
	
	return result;
}

int TestSpellChecker()
{
	int result = 0;

	result = (SPELL_CHECKER_SUCCESS != SpellChecker());
	
	return result;
}