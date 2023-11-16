#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include <errno.h>			/* errno */
#include "circular_buffer.h"

#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"
#define BLUE_COLOR		"\x1b[0;34m"

enum EXIT_STATUS {FAILED, SUCCESS, DONE};

struct circ_buffer
{
	size_t read_offset;
	size_t capacity;
	size_t size;
	char buffer[1];
};

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

int TestCBufferCreate();
int TestCBufferIsEmpty();
int TestCBufferSize();
int TestCBufferFreeSpace();
int TestCBufferWrite();
int TestCBufferRead();
int TestIntegrationCBufferReadAndWrite();
int TestCBufferDestroy();

circ_buffer_t *cbuf = NULL;
char a[] = "Hello! This is a test for cbuf functions!";
char b[42] = {0};

int main(int argc, char *argv[])
{
	RUN_TEST(TestCBufferCreate(),					"CBufferCreate");
	RUN_TEST(TestCBufferIsEmpty(),					"CBufferIsEmpty");
	RUN_TEST(TestCBufferSize(),						"CBufferSize");
	RUN_TEST(TestCBufferFreeSpace(),				"CBufferFreeSpace");
	RUN_TEST(TestCBufferWrite(),					"CBufferWrite");
	RUN_TEST(TestCBufferRead(),						"CBufferRead");
	RUN_TEST(TestIntegrationCBufferReadAndWrite(),	"IntegrationCBufferReadAndWrite");
	RUN_TEST(TestCBufferDestroy(),		"CBufferDestroy");

	(void) argc;
	(void) argv;

    return 0;
}

int TestCBufferCreate()
{
	cbuf = CBufferCreate(21);
	return (NULL != cbuf);
}

int TestCBufferIsEmpty()
{	
	return (1 == CBufferIsEmpty(cbuf));
}

int TestCBufferSize()
{	
	return (0 == CBufferSize(cbuf));
}

int TestCBufferFreeSpace()
{	
	return (21 == CBufferFreeSpace(cbuf));
}

int TestCBufferWrite()
{
	
	ssize_t c = 0;
	int d = 0;
	
	c = CBufferWrite(cbuf, (void *)a, 10);
	d += (10 == c && 0 == strncmp(a, cbuf->buffer, 10));
	c = CBufferWrite(cbuf, (void *)&a[10], 10);
	d += (10 == c && 0 == strncmp(&a[10], &cbuf->buffer[10], 10));
	c = CBufferWrite(cbuf, (void *)&a[20], 10);
	d += (1 == c);
	c = CBufferWrite(cbuf, (void *)&a[30], 10);
	if(-1 == c && ENOSPC == errno)
	{
		d += 1;
	}
	return (4 == d);
}

int TestCBufferRead()
{
	ssize_t c = 0;
	int d = 0;
	
	c = CBufferRead(cbuf, (void *)b, 10);
	d += (10 == c && 0 == strncmp(b, a, 10));
	c = CBufferRead(cbuf, (void *)&b[10], 10);
	d += (10 == c && 0 == strncmp(&b[10], &a[10], 10));
	c = CBufferRead(cbuf, (void *)b, 10);
	d += (1 == c);
	c = CBufferRead(cbuf, (void *)b, 10);;
	if(-1 == c && ENODATA == errno)
	{
		d += 1;
	}
	return (4 == d);
}

int TestIntegrationCBufferReadAndWrite()
{
	ssize_t c = 0;
	int d = 0;
	
	c = CBufferWrite(cbuf, (void *)a, 10);
	d += (10 == c);
	c = CBufferRead(cbuf, (void *)b, 10);
	d += (10 == c && 0 == strncmp(a, b, 10));
	c = CBufferWrite(cbuf, (void *)&a[10], 10);
	d += (10 == c);
	c = CBufferRead(cbuf, (void *)&b[10], 10);
	d += (10 == c && 0 == strncmp(&a[10], &b[10], 10));
	c = CBufferWrite(cbuf, (void *)&a[20], 10);
	d += (10 == c);
	c = CBufferRead(cbuf, (void *)&b[20], 10);
	d += (10 == c && 0 == strncmp(&a[20], &b[20], 10));
	c = CBufferWrite(cbuf, (void *)&a[30], 10);
	d += (10 == c);
	c = CBufferRead(cbuf, (void *)&b[30], 10);
	d += (10 == c && 0 == strncmp(&a[30], &b[30], 10));
	c = CBufferWrite(cbuf, (void *)&a[40], 3);
	d += (3 == c);
	c = CBufferRead(cbuf, (void *)&b[40], 3);
	d += (3 == c && 0 == strncmp(&a[40], &b[40], 3));
	
	return (10 == d);
}

int TestCBufferDestroy()
{
	CBufferDestroy(cbuf);
	return DONE;
}
