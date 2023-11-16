#include <stdio.h>	/* printf() */
#include <string.h>
#include "bit_array.h"

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

int TestBitArraySetAll();
int TestBitArrayResetAll();
int TestBitArraySetOn();
int TestBitArraySetOff();
int TestBitArraySetBit();
int TestBitArrayGetVal();
int TestBitArrayFlip();
int TestBitArrayMirror();
int TestBitArrayRotR();
int TestBitArrayRotL();
int TestBitArrayCountOn();
int TestBitArrayCountOff();
int TestBitArrayToString();
int TestBitArrayMirrorLut();
int TestBitArrayCountOnLut();

int main(int argc, char *argv[])
{
	RUN_TEST(TestBitArraySetAll(), "BitArraySetAll");
	RUN_TEST(TestBitArrayResetAll(), "BitArrayResetAll");
	RUN_TEST(TestBitArraySetOn(), "BitArraySetOn");
	RUN_TEST(TestBitArraySetOff(), "BitArraySetOff");
	RUN_TEST(TestBitArraySetBit(), "BitArraySetBit");
	RUN_TEST(TestBitArrayGetVal(), "BitArrayGetVal");
	RUN_TEST(TestBitArrayFlip(), "BitArrayFlip");
	RUN_TEST(TestBitArrayMirror(), "BitArrayMirror");
	RUN_TEST(TestBitArrayRotR(), "BitArrayRotR");
	RUN_TEST(TestBitArrayRotL(), "BitArrayRotL");
	RUN_TEST(TestBitArrayCountOn(), "BitArrayCountOn");
	RUN_TEST(TestBitArrayCountOff(), "BitArrayCountOff");
	RUN_TEST(TestBitArrayToString(), "BitArrayToString");
	RUN_TEST(TestBitArrayMirrorLut(), "BitArrayMirrorLut");
	RUN_TEST(TestBitArrayCountOnLut(), "BitArrayCountOnLut");

	(void) argc;
	(void) argv;
	
	return 0;
}

int TestBitArraySetAll()
{
	return (bitarr_t)(~0) == BitArraySetAll(64738495763928);
}

int TestBitArrayResetAll()
{
	return 0 == BitArrayResetAll(64738495763928);
}

int TestBitArraySetOn()
{
	return (BitArraySetOn(64738495763928, 9) == 64738495764440) &&
			(BitArraySetOn(8, 3) == 8);
}

int TestBitArraySetOff()
{
	return (BitArraySetOff(64738495763928, 9) == 64738495763928) &&
			(BitArraySetOff(8, 3) == 0);
}

int TestBitArraySetBit()
{
	return (BitArraySetBit(0, 3, 1) == 8) &&
			(BitArraySetBit(0, 3, 0) == 0) &&
			(BitArraySetBit(8, 3, 1) == 8);
}

int TestBitArrayGetVal()
{
	return (BitArrayGetVal(5, 2) == 1) &&
			(BitArrayGetVal(5, 3) == 0);
}

int TestBitArrayFlip()
{
	return (BitArrayFlip(5, 1) == 7) &&
			(BitArrayFlip(5, 2) == 1);
}

int TestBitArrayMirror()
{
	return (BitArrayMirror(5) == 0xA000000000000000) &&
			(BitArrayMirror(0) == 0);
}

int TestBitArrayRotR()
{
	return (BitArrayRotR(10, 1) == 5) &&
			(BitArrayRotR(0, 3) == 0);
}

int TestBitArrayRotL()
{
	return (BitArrayRotL(5, 1) == 10) &&
			(BitArrayRotL(0, 3) == 0);
}

int TestBitArrayCountOn()
{
	return (BitArrayCountOn(5) == 2) &&
			(BitArrayCountOn(0) == 0);
}

int TestBitArrayCountOff()
{
	return (BitArrayCountOff(5) == 62) &&
			(BitArrayCountOff(0) == 64);
}

int TestBitArrayToString()
{
	char a[65] = {0};
	char b[65] = {0};
	char c[] = 
			"0000000000000000000000000000000000000000000000000000000000000101";
	char d[] = 
			"0000000000000000000000000000000000000000000000000000000000000000";
	
	BitArrayToString(5, a);
	BitArrayToString(0, b);
	
	return (0 == strcmp(a,c)) &&
			(0 == strcmp(b,d));
}

int TestBitArrayMirrorLut()
{
	return (BitArrayMirrorLut(5) == 0xA000000000000000) &&
			(BitArrayMirrorLut(0) == 0);
}

int TestBitArrayCountOnLut()
{
	return (BitArrayCountOnLut(5) == 2) &&
			(BitArrayCountOnLut(0) == 0);
}
