#include <stdio.h> 	/* printf()			*/
#include <stdlib.h>	/* malloc(), free()	*/
#include "fsa.h"

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

int TestFSASuggestSegmentSize();
int TestFSAIntegration();

int main(int argc, char *argv[])
{
	RUN_TEST(TestFSASuggestSegmentSize(),		"FSASuggestSegmentSize");
	RUN_TEST(TestFSAIntegration(),				"FSAIntegration");

	(void) argc;
	(void) argv;

    return 0;
}

int TestFSASuggestSegmentSize()
{
	int result = 0;
	
	result += (56 == FSASuggestSegmentSize(3, 5));
	result += (56 == FSASuggestSegmentSize(8, 5));
	result += (96 == FSASuggestSegmentSize(12, 5));
	result += (96 == FSASuggestSegmentSize(15, 5));
	result += (24 == FSASuggestSegmentSize(1, 1));
	
	return (5 == result);
}

int TestFSAIntegration()
{
	fsa_t *test_fsa = NULL;
	void *saved_memory = malloc(sizeof(char) * 60);
	void *memory_pool = NULL;
	void *block1 = NULL;
	void *block2 = NULL;
	void *block3 = NULL;
	void *block4 = NULL;
	void *block5 = NULL;
	char *ptr = NULL;
	size_t wordsize = 8;
	int result = 0;
	
	if(NULL == saved_memory)
	{
		return 0;
	}
	ptr = (char *)saved_memory;
	while(0 != ((size_t)ptr & (wordsize - 1)))
	{
		++ptr;
	}
	memory_pool = (void *)ptr;
	test_fsa = FSAInit(memory_pool, 48, 3);
	result += (4 == FSACountFree(test_fsa));
	
	test_fsa = FSAInit(memory_pool, 48, 8);
	result += (4 == FSACountFree(test_fsa));
	
	test_fsa = FSAInit(memory_pool, 50, 8);
	result += (4 == FSACountFree(test_fsa));
	
	test_fsa = FSAInit(memory_pool, 48, 12);
	result += (2 == FSACountFree(test_fsa));
	
	test_fsa = FSAInit(memory_pool, 48, 15);
	result += (2 == FSACountFree(test_fsa));
	
	++ptr;
	memory_pool = (void *)ptr;
	test_fsa = FSAInit(memory_pool, 48, 3);
	result += (3 == FSACountFree(test_fsa));
	
	--ptr;
	memory_pool = (void *)ptr;
	test_fsa = FSAInit(memory_pool, 48, 8);
	block1 = FSAAlloc(test_fsa);
	result += (3 == FSACountFree(test_fsa));
	result += (NULL != block1);
	block2 = FSAAlloc(test_fsa);
	result += (2 == FSACountFree(test_fsa));
	result += (NULL != block2);
	block3 = FSAAlloc(test_fsa);
	result += (1 == FSACountFree(test_fsa));
	result += (NULL != block3);
	block4 = FSAAlloc(test_fsa);
	result += (0 == FSACountFree(test_fsa));
	result += (NULL != block4);
	block5 = FSAAlloc(test_fsa);
	result += (0 == FSACountFree(test_fsa));
	result += (NULL == block5);
	
	FSAFree(test_fsa, block2);
	result += (1 == FSACountFree(test_fsa));
	FSAFree(test_fsa, block4);
	result += (2 == FSACountFree(test_fsa));
	FSAFree(test_fsa, block1);
	result += (3 == FSACountFree(test_fsa));
	FSAFree(test_fsa, block3);
	result += (4 == FSACountFree(test_fsa));
	
	block1 = FSAAlloc(test_fsa);
	result += (block1 == block3);
	
	free(saved_memory);
	
	return (21 == result);
}
