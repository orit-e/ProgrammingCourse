#include <stdio.h> /* printf */
#include <string.h> /* strlen, strcmp */
#include "mem.h"

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

int TestMemset();
int TestMemcpy();
int TestMemmove();

int main(int argc, char *argv[])
{
	RUN_TEST(TestMemset(),			"Memset");
	RUN_TEST(TestMemcpy(),			"Memcpy");
	RUN_TEST(TestMemmove(),			"Memmove");

	(void) argc;
	(void) argv;
	
	return 0;
}

int TestMemset()
{
	int c1 = '*';
	char c2[] = "password";
	char c3 = '*';
	int d = 0;

	Memset((void*) c2, c1, strlen(c2));
	Memset((void*) &c3, c1, 1);
	Memset((void*) &d, c1, 2);
	
	return	0 		== strcmp(c2, "********")		&&
			'*' 	== c3							&&
			10794  	== d							;
}

int TestMemcpy()
{
	char c1[6] = {0};
	char c2[] = "helloworld";

	Memcpy((void*) c1, (void*) c2, 5);
	Memcpy((void*) c2, (const void*) (c2 + 5), 5);

	return	0 == strcmp(c1, "hello")		&&
			0 == strcmp(c2, "worldworld")	;
}

int TestMemmove()
{
	char c1[6] = {0};
	char c2[11] = "helloworld";
	char c3[] = "00001234567890000";
	char c4[] = "00001234567890000";

	Memmove((void*) c1, (void*) c2, 5);
	Memmove((void*) c2, (const void*) (c2 + 5), 5);
	Memmove((void*) c3, (const void*) (c3 + 4), 9);
	Memmove((void*) (c4 + 8), (const void*) (c4 + 4), 9);

	return	0 == strcmp(c1, "hello")				&&
			0 == strcmp(c2, "worldworld")			&&
			0 == strcmp(c3, "12345678967890000")	&&
			0 == strcmp(c4, "00001234123456789")	;
}
