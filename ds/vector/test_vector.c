#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include "vector.h"

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

int TestVectorCreate();
int TestVectorPush();
int TestVectorGetElement();
int TestVectorPop();
int TestVectorSize();
int TestVectorCapacity();
int TestVectorSetElement();
int TestVectorExpand();
int TestVectorShrink();
int TestVectorDestroy();



vector_t *vector= NULL;
int d = 10;
char c = 'a';
char *s = "hello";

int main(int argc, char *argv[])
{
	RUN_TEST(TestVectorCreate(), 		"VectorCreate" 		);
	RUN_TEST(TestVectorPush(),			"VectorPush"		);
	RUN_TEST(TestVectorGetElement(),	"VectorGetElement" 	);
	RUN_TEST(TestVectorPop(),			"VectorPop" 		);
	RUN_TEST(TestVectorSize(),			"VectorSize"		);
	RUN_TEST(TestVectorCapacity(),		"VectorCapacity"	);
	RUN_TEST(TestVectorSetElement(),	"VectorSetElement"	);
	RUN_TEST(TestVectorExpand(),		"VectorExpand"		);
	RUN_TEST(TestVectorShrink(),		"VectorShrink"		);
	RUN_TEST(TestVectorDestroy(),		"VectorDestroy"		);

	(void) argc;
	(void) argv;
	
	return 0;
}

int TestVectorCreate()
{
	vector = VectorCreate((size_t) 3);
	return (8 == VectorCapacity(vector));
}

int TestVectorPush()
{
	int i = 0;
	
	for(i = 0; i < 3; ++i)
	{
		VectorPush(vector, (void*) &d);
		VectorPush(vector, (void*) s);
		VectorPush(vector, (void*) &c);
	}
	return (16 == VectorCapacity(vector));
}

int TestVectorGetElement()
{
	return (0 == strcmp((char*)VectorGetElement(vector, 1), "hello")); 
}

int TestVectorPop()
{
	VectorPop(vector);
	VectorPop(vector);
	VectorPop(vector);
	VectorPop(vector);
	VectorPop(vector);
	return (8 == VectorCapacity(vector));
}

int TestVectorSize()
{
	VectorPush(vector, (void*) &d);
	VectorPush(vector, (void*) &c);
	return (6 == VectorSize(vector));
}

int TestVectorCapacity()
{
	return (8 == VectorCapacity(vector));
}

int TestVectorSetElement()
{
	VectorSetElement(vector, 0, s);
	
	return (0 == strcmp((char*)VectorGetElement(vector, 0), "hello")); 
}

int TestVectorExpand()
{
	return (0 == VectorExpand(vector, 10));
}

int TestVectorShrink()
{
	return (0 == VectorShrink(vector));
}	

int TestVectorDestroy()
{
	VectorDestroy(vector);
	vector = NULL;
	return 1;
}
