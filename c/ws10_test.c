#include <stdio.h> /* printf */
#include <string.h> /* strlen, strcmp */
#include <stdlib.h> /* atoi, strtol */
#include <limits.h> /* INT_MAX, INT_MIN */
#include "ws10.h"

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

#define IS_LITTLE_ENDIAN() (((*(short *)"21") & 0xFF) == '2')



int TestItoaUpTo36Base();
int TestAtoiUpTo36Base();
void TestPrintFirstAndSecondNotThird();
void TestEndianFunctio();
void TestEndianMacro();

int main(int argc, char *argv[])
{
	RUN_TEST(TestItoaUpTo36Base(),	"ItoaUpTo36Base");
	RUN_TEST(TestAtoiUpTo36Base(),	"AtoiUpTo36Base");
	TestPrintFirstAndSecondNotThird();
	TestEndianFunctio();
	TestEndianMacro();

	(void) argc;
	(void) argv;
	
	return 0;
}

int TestItoaUpTo36Base()
{
	char s0[20] = {0}, s1[20] = {0}, s2[20] = {0}, s3[20] = {0}, s4[20] = {0};

	ItoaUpTo36Base(1000,	s0, 8);
	ItoaUpTo36Base(INT_MAX,	s1, 10);
	ItoaUpTo36Base(INT_MIN,	s2, 20);
	ItoaUpTo36Base(0,		s3, 2);
	ItoaUpTo36Base(37,		s4, 36);

	return 	0 == strcmp(ItoaUpTo36Base(1000,    s0, 8),  "1750")		&&
			0 == strcmp(ItoaUpTo36Base(INT_MAX, s1, 10), "2147483647")	&&
			0 == strcmp(ItoaUpTo36Base(INT_MIN,	s2, 20), "-1DB1F928")	&&
			0 == strcmp(ItoaUpTo36Base(0,		 s3, 2),  "0")			&&
			0 == strcmp(ItoaUpTo36Base(37,	 	 s4, 36), "11")			;
}

int TestAtoiUpTo36Base()
{
	char *s1 = "1000";
	char *s2 = "0";
	char *s3 = "2147483647"; /* INT_MAX */
	char *s4 = "XyZ";
	char *s5 = "-3F";
	char *p1 = &(s4[3]);
	char *p2 = &(s5[3]);

	return 	AtoiUpTo36Base(s1, 10) == atoi(s1)					&&
		 	AtoiUpTo36Base(s2, 10) == atoi(s2)					&&
			AtoiUpTo36Base(s3, 10) == atoi(s3)					&&
			AtoiUpTo36Base(s4, 36) == (int) strtol(s4, &p1, 36)	&&
			AtoiUpTo36Base(s5, 16) == (int) strtol(s5, &p2, 16)	;
}

void TestPrintFirstAndSecondNotThird()
{
	char *s1 = "1234567890";
	char *s2 = "1 3 5 7 9 ";
	char *s3 = "1       9 ";
	
	printf("PrintFirstAndSecondNotThird - should print '357': ");	
	PrintFirstAndSecondNotThird(s1, s2, s3);	
}

void TestEndianFunctio()
{
	if(0 == IsLittleEndian())
	{
		printf("Little Endian!\n");
	}
	else
	{
		printf("Big Endian!\n");
	}
}
	
void TestEndianMacro()
{
	if(1 == IS_LITTLE_ENDIAN())
	{
		printf("Little Endian!\n");
	}
	else
	{
		printf("Big Endian!\n");
	}
}	
	
	
