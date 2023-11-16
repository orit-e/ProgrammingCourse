#include <stdio.h> 	/* printf()			*/
#include <stdlib.h>	/* malloc(), free()	*/
#include "vsa.h"

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

int GDTestVSAIntegration1();
int GDTestVSAIntegration2();
int GDTestVSAIntegration3();
int GDTestVSAIntegration4();
int GDTestVSAIntegration5();

int GCTestVSAIntegration1();
int GCTestVSAIntegration2();
int GCTestVSAIntegration3();
int GCTestVSAIntegration4();
int GCTestVSAIntegration5();

int main(int argc, char *argv[])
{
	#ifndef NDEBUG
		RUN_TEST(GDTestVSAIntegration1(),		"GDTestVSAIntegration1");
		RUN_TEST(GDTestVSAIntegration2(),		"GDTestVSAIntegration2");
		RUN_TEST(GDTestVSAIntegration3(),		"GDTestVSAIntegration3");
		RUN_TEST(GDTestVSAIntegration4(),		"GDTestVSAIntegration4");
		RUN_TEST(GDTestVSAIntegration5(),		"GDTestVSAIntegration5");
	#else
		RUN_TEST(GCTestVSAIntegration1(),		"GCTestVSAIntegration1");
		RUN_TEST(GCTestVSAIntegration2(),		"GCTestVSAIntegration2");
		RUN_TEST(GCTestVSAIntegration3(),		"GCTestVSAIntegration3");
		RUN_TEST(GCTestVSAIntegration4(),		"GCTestVSAIntegration4");
		RUN_TEST(GCTestVSAIntegration5(),		"GCTestVSAIntegration5");
	#endif

	(void) argc;
	(void) argv;

    return 0;
}

int GDTestVSAIntegration1()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 300);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 300);
	result += (272 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 3);
	result += (NULL != chunk1);
	result += (248 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk1);
	result += (272 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk2);
	
	free(memory_segment);
	
	return (4 == result);
}

int GDTestVSAIntegration2()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 300);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	void *chunk3 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 300);
	result += (272 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 3);
	result += (NULL != chunk1);
	result += (248 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 11);
	result += (NULL != chunk2);
	result += (216 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 15);
	result += (NULL != chunk3);
	result += (184 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk1);
	VSAFree(chunk2);
	VSAFree(chunk3);
	result += (272 == VSALargestChunkAvailable(test_vsa));
	
	free(memory_segment);
	
	return (8 == result);
}

int GDTestVSAIntegration3()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 100);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	void *chunk3 = NULL;
	void *chunk4 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 100);
	result += (72 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk1);
	result += (48 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk2);
	result += (24 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk3);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	chunk4 = VSAAlloc(test_vsa, 8);
	result += (NULL == chunk4);
	VSAFree(chunk1);
	VSAFree(chunk2);
	chunk4 = VSAAlloc(test_vsa, 32);
	result += (NULL != chunk4);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	
	free(memory_segment);
	
	return (10 == result);
}

int GDTestVSAIntegration4()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 108);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	void *chunk3 = NULL;
	void *chunk4 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 108);
	result += (80 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk1);
	result += (56 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk2);
	result += (32 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 33);
	result += (NULL == chunk3);
	result += (32 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 17);
	result += (NULL != chunk3);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk2);
	chunk4 = VSAAlloc(test_vsa, 13);
	result += (NULL == chunk4);
	result += (8 == VSALargestChunkAvailable(test_vsa));
	chunk4 = VSAAlloc(test_vsa, 3);
	result += (NULL != chunk4);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	
	free(memory_segment);
	
	return (13 == result);
}

int GDTestVSAIntegration5()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 100);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 100);
	result += (72 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 16);
	result += (NULL != chunk1);
	result += (40 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 24);
	result += (NULL != chunk2);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	free(memory_segment);
	
	return (5 == result);
}
/******************************************************************************/
int GCTestVSAIntegration1()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 60);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 60);
	result += (40 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 3);
	result += (NULL != chunk1);
	result += (24 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk1);
	result += (40 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk2);
	
	free(memory_segment);
	
	return (4 == result);
}

int GCTestVSAIntegration2()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 300);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	void *chunk3 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 300);
	result += (280 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 3);
	result += (NULL != chunk1);
	result += (264 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 11);
	result += (NULL != chunk2);
	result += (240 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 15);
	result += (NULL != chunk3);
	result += (216 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk1);
	VSAFree(chunk2);
	VSAFree(chunk3);
	result += (280 == VSALargestChunkAvailable(test_vsa));
	
	free(memory_segment);
	
	return (8 == result);
}

int GCTestVSAIntegration3()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 72);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	void *chunk3 = NULL;
	void *chunk4 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 72);
	result += (56 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk1);
	result += (40 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk2);
	result += (24 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 16);
	result += (NULL != chunk3);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	chunk4 = VSAAlloc(test_vsa, 8);
	result += (NULL == chunk4);
	VSAFree(chunk1);
	VSAFree(chunk2);
	chunk4 = VSAAlloc(test_vsa, 24);
	result += (NULL != chunk4);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	
	free(memory_segment);
	
	return (10 == result);
}

int GCTestVSAIntegration4()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 80);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	void *chunk3 = NULL;
	void *chunk4 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 80);
	result += (64 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk1);
	result += (48 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 8);
	result += (NULL != chunk2);
	result += (32 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 33);
	result += (NULL == chunk3);
	result += (32 == VSALargestChunkAvailable(test_vsa));
	chunk3 = VSAAlloc(test_vsa, 17);
	result += (NULL != chunk3);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	VSAFree(chunk2);
	chunk4 = VSAAlloc(test_vsa, 13);
	result += (NULL == chunk4);
	result += (8 == VSALargestChunkAvailable(test_vsa));
	chunk4 = VSAAlloc(test_vsa, 3);
	result += (NULL != chunk4);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	
	free(memory_segment);
	
	return (13 == result);
}

int GCTestVSAIntegration5()
{
	vsa_t *test_vsa = NULL;
	void *memory_segment = malloc(sizeof(char) * 80);
	void *chunk1 = NULL;
	void *chunk2 = NULL;
	int result = 0;
	
	if(NULL == memory_segment)
	{
		return 0;
	}
	test_vsa = VSAInit(memory_segment, 80);
	result += (64 == VSALargestChunkAvailable(test_vsa));
	chunk1 = VSAAlloc(test_vsa, 24);
	result += (NULL != chunk1);
	result += (32 == VSALargestChunkAvailable(test_vsa));
	chunk2 = VSAAlloc(test_vsa, 24);
	result += (NULL != chunk2);
	result += (0 == VSALargestChunkAvailable(test_vsa));
	free(memory_segment);
	
	return (5 == result);
}


