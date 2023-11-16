#include <stdio.h>	/* printf() */
#include <limits.h> /* INT_MAX */
#include "bitwise.h"

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

int TestPow2();
int TestIsPowOf2();
int TestIsPowOf2ver2();
int TestAdd1();
int TestByteMirror1();
int TestByteMirror2();
int TestIs2And6();
int TestIs2Or6();
int TestSwap35();
int TestClosest16();
int TestSwap();
int TestCountSetBits();
int TestCountSetBits2();

int main(int argc, char *argv[])
{
	/*unsigned int is_3_bits[] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};*/

	RUN_TEST(TestPow2(),			"Pow2");
	RUN_TEST(TestIsPowOf2(),		"IsPowerOf2");
	RUN_TEST(TestIsPowOf2ver2(),	"IsPowerOf2ver2");
	RUN_TEST(TestAdd1(),			"Add1");
	/*Is3Bits(is_3_bits, (size_t) 16));*/
	RUN_TEST(TestByteMirror1(),		"ByteMirror1"); 
	RUN_TEST(TestByteMirror2(),		"ByteMirror2");
	RUN_TEST(TestIs2And6(),			"Is2And6");
	RUN_TEST(TestIs2Or6(),			"Is2Or6");
	RUN_TEST(TestSwap35(),			"Swap35");
	RUN_TEST(TestClosest16(),		"Closest16");
	RUN_TEST(TestCountSetBits(),	"CountSetBits");
	RUN_TEST(TestCountSetBits2(),	"CountSetBits2");
	/*PrintFloatBits(1.2);
	PrintFloatBits(1.0);
	PrintFloatBits(0.0001);
	PrintFloatBits(1000.0);*/

	(void) argc;
	(void) argv;
	
	return 0;
}

int TestPow2()
{
	return 	Pow2(4, 4)			== 64			&&
			Pow2(1234, 0)		== 1234			&&
			Pow2(10, 32)		== 42949672960	;
}

int TestIsPowOf2()
{
	return 	IsPowOf2(8)				== 1	&&
			IsPowOf2(1024)			== 1	&&
			IsPowOf2(1025)			== 0	&&
			IsPowOf2(0x80000000)	== 1	&&
			IsPowOf2(1023)			== 0	;
}

int TestIsPowOf2ver2()
{
	return 	IsPowOf2ver2(8)		== 1	&&
			IsPowOf2ver2(1024)	== 1	&&
			IsPowOf2ver2(1025)	== 0	&&
			IsPowOf2ver2(1023)	== 0	;
}

int TestAdd1()
{
	return 	Add1(10)		== 11					&&
			Add1(INT_MAX)	== INT_MAX + (long) 1	&&
			Add1(0)			== 1					;
}

int TestByteMirror1()
{
	return 	ByteMirror1(1)		== 128		&&
			ByteMirror1(129)	== 129		&&
			ByteMirror1(0)		== 0		&&
			ByteMirror1(255)	== 255		&&
			ByteMirror1(234)	== 87		;
}

int TestByteMirror2()
{
	return 	ByteMirror2(1)		== 128		&&
			ByteMirror2(129)	== 129		&&
			ByteMirror2(0)		== 0		&&
			ByteMirror2(255)	== 255		&&
			ByteMirror2(234)	== 87		;
}

int TestIs2And6()
{
	return 	Is2And6('U')		== 1	&&
			Is2And6('S')		== 0	;
}

int TestIs2Or6()
{
	return 	Is2Or6('S')		== 1	&&
			Is2Or6('3')		== 0	;
}

int TestSwap35()
{
	return 	Swap35('U')		== 'U'	&&
			Swap35('H')		== '`'	;
}

int TestClosest16()
{
	return 	Closest16(255)		== 240				&&
			Closest16(85)		== 80				&&
			Closest16(16)		== 16				&&
			Closest16(0)		== 0				&&
			Closest16(INT_MAX)  == INT_MAX - 15 	&&
			Closest16(15)		== 0				;
}

int TestSwap()
{
	int n1 = 1, n2 = 2, big1 = INT_MAX, big2 = 1000;
   	int orig_n1 = n1, orig_n2 = n2, orig_big1 = big1, orig_big2 = big2;	

	Swap(&n1, &n2);
	Swap(&big1, &big2);

	return 	n1		== 		orig_n2		&&
			n2		== 		orig_n1		&&
			big1	== 		orig_big2	&&
			big2	== 		orig_big1	;
}

int TestCountSetBits()
{
	return 	CountSetBits(256)		== 1		&&
			CountSetBits(INT_MAX)	== 31		&&
			CountSetBits(-1) 		== 32		&&
			CountSetBits(0) 		== 0		&&
			CountSetBits(255)		== 8		;
}

int TestCountSetBits2()
{
	return 	CountSetBits2(256) 		== 1		&&
			CountSetBits2(INT_MAX) 	== 31		&&
			CountSetBits2(-1) 		== 32		&&
			CountSetBits2(0) 		== 0		&&
			CountSetBits2(255)		== 8		;
}
