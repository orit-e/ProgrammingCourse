#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "calc.h"

#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"
#define BLUE_COLOR		"\x1b[0;34m"

enum EXIT_STATUS {TEST_FAILED, TEST_SUCCESS, TEST_DONE};

#define RUN_TEST(result, name) {\
			printf("test %s: ", name);\
			switch (result)\
			{\
				case TEST_SUCCESS:\
					printf(GREEN_COLOR);\
					printf("Success\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case TEST_FAILED:\
					printf(RED_COLOR);\
					printf("Fail\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case TEST_DONE:\
					printf(BLUE_COLOR);\
					printf("Done\n");\
					printf(DEFAULT_COLOR);\
			};\
		}

int TestSumFunc();
int TestSubtractFunc();
int TestSumAndSubstractIntegraton();
int TestMultiplyFunc();
int TestDivideFunc();
int TestMultiplyAndDivideIntegraton();
int TestAllOpsButPowerIntegraton();
int TestPowerFunc();
int TestAllOpsIntegraton();
int TestParanHandling();
int TestUnary();
int TestErrors();
/*int TestAllOpsButPowerIntegraton();
*/

int main(int argc, char *argv[])
{
	RUN_TEST(TestSumFunc(),							"TestSumFunc");
	RUN_TEST(TestSubtractFunc(),					"TestSubtractFunc");
	RUN_TEST(TestSumAndSubstractIntegraton(),		"TestSumAndSubstractIntegraton");
	RUN_TEST(TestMultiplyFunc(),					"TestMultiplyFunc");
	RUN_TEST(TestDivideFunc(),						"TestDivideFunc");
	RUN_TEST(TestMultiplyAndDivideIntegraton(),		"TestMultiplyAndDivideIntegraton");
	RUN_TEST(TestAllOpsButPowerIntegraton(),		"TestAllOpsButPowerIntegraton");
	RUN_TEST(TestPowerFunc(),						"TestPowerFunc");
	RUN_TEST(TestAllOpsIntegraton(),				"TestAllOpsIntegraton");
	RUN_TEST(TestParanHandling(),					"TestParanHandling");
	RUN_TEST(TestUnary(),							"TestUnary");
	RUN_TEST(TestErrors(),							"TestErrors");

	(void) argc;
	(void) argv;

    return 0;
}

int TestSumFunc()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "4 + 5";
	char *exp2 = "4 + 5 + 3";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (9 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (12 == answer);
	
	return (4 == result);
}

int TestSubtractFunc()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "5 - 4";
	char *exp2 = "5 - 5";
	char *exp3 = "4 - 5";
	char *exp4 = "9 - 3 - 7";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (1 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (0 == answer);
	result += (CALC_SUCCESS == Calculate(exp3, &answer));
	result += (-1 == answer);
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (-1 == answer);
	
	return (8 == result);
}

int TestSumAndSubstractIntegraton()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "5 + 4 - 3";
	char *exp2 = "5 - 2 + 6";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (6 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (9 == answer);
	
	return (4 == result);
}

int TestMultiplyFunc()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "4 * 5";
	char *exp2 = "4 * 5 * 3";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (20 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (60 == answer);
	
	return (4 == result);
}

int TestDivideFunc()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "5 / 4";
	char *exp2 = "5 / 5";
	char *exp3 = "4 / 5";
	char *exp4 = "12 / 3 / 4";
	char *exp5 = "12 / 0";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (1.25 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (1 == answer);
	result += (CALC_SUCCESS == Calculate(exp3, &answer));
	result += (0.8 == answer);
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (1 == answer);
	result += (DIVIDE_BY_ZERO_ERROR == Calculate(exp5, &answer));
	
	return (9 == result);
}

int TestMultiplyAndDivideIntegraton()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "5 * 4 / 2";
	char *exp2 = "14 / 2 * 6";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (10 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (42 == answer);
	
	return (4 == result);
}


int TestAllOpsButPowerIntegraton()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "5 + 4 * 3";
	char *exp2 = "5 * 5 - 2";
	char *exp3 = "16 - 8 / 2";
	char *exp4 = "1 - 2 / 2 * 2 * 2";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (17 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (23 == answer);
	result += (CALC_SUCCESS == Calculate(exp3, &answer));
	result += (12 == answer);
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (-3 == answer);
	
	return (8 == result);
}

int TestPowerFunc()
{
	double answer = 0;
	int result = 0;
	char *exp1 = "5 ^ 4";
	char *exp2 = "0 ^ 5";
	char *exp3 = "3 ^ 0";
	char *exp4 = "1.5 ^ 2";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (625 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (0 == answer);
	result += (CALC_SUCCESS == Calculate(exp3, &answer));
	result += (1 == answer);
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (2.25 == answer);
	
	return (8 == result);
}

int TestAllOpsIntegraton()
{	
	double answer = 0;
	int result = 0;
	char *exp1 = "5 +4 ^ 2";
	char *exp2 = "5* 5 ^ 2";
	char *exp3 = "16 - 8 ^2";
	char *exp4 = "64 / 2 ^ 3";
	char *exp5 = "5 ^ 2 * 5";
	char *exp6 = "2 ^ 4 - 8";
	char *exp7 = "5 ^ 2 + 4";
	char *exp8 = "3 ^ 4 / 9";
	char *exp9 = "16 - 4 * 2 ^ 3";
	char *exp10 = "4^3^2";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (21 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (125 == answer);
	result += (CALC_SUCCESS == Calculate(exp3, &answer));
	result += (-48 == answer);
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (8 == answer);
	result += (CALC_SUCCESS == Calculate(exp5, &answer));
	result += (125 == answer);
	result += (CALC_SUCCESS == Calculate(exp6, &answer));
	result += (8 == answer);
	result += (CALC_SUCCESS == Calculate(exp7, &answer));
	result += (29 == answer);
	result += (CALC_SUCCESS == Calculate(exp8, &answer));
	result += (9 == answer);
	result += (CALC_SUCCESS == Calculate(exp9, &answer));
	result += (-16 == answer);
	result += (CALC_SUCCESS == Calculate(exp10, &answer));
	result += (262144 == answer);
	
	return (20 == result);
}

int TestParanHandling()
{	
	double answer = 0;
	int result = 0;
	char *exp1 = "( 5 + 4 )";
	char *exp2 = "5 * (5 + 2)";
	char *exp3 = "(16 -8^ 2)";
	char *exp4 = "(64 / 2) - 3";
	char *exp5 = "64 + 2) - 3";
	char *exp6 = "5 + (2 - 3";
	char *exp7 = "5 + ((2 - 3) + 4)";
	char *exp8 = "5 + 3(5)";
	char *exp9 = "(((5 + 3)))";
	char *exp10 = "(5)3";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (9 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (35 == answer);
	result += (CALC_SUCCESS == Calculate(exp3, &answer));
	result += (-48 == answer);
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (29 == answer);
	result += (PARAN_IMBALANCE == Calculate(exp5, &answer));
	result += (PARAN_IMBALANCE == Calculate(exp6, &answer));
	result += (CALC_SUCCESS == Calculate(exp7, &answer));
	result += (8 == answer);
	result += (CALC_SUCCESS == Calculate(exp8, &answer));
	result += (20 == answer);
	result += (CALC_SUCCESS == Calculate(exp9, &answer));
	result += (8 == answer);
	result += (CALC_SUCCESS == Calculate(exp10, &answer));
	result += (15 == answer);
	
	return (18 == result);
}

int TestUnary()
{	
	double answer = 0;
	int result = 0;
	char *exp1 = "5 + +4";
	char *exp2 = "5 *-5";
	char *exp3 = "16 - +";
	char *exp4 = "-64 / 2";
	char *exp5 = "+64+ 2";
	char *exp6 = "+ + 2";
	char *exp7 = "+ / 2";
	
	result += (CALC_SUCCESS == Calculate(exp1, &answer));
	result += (9 == answer);
	result += (CALC_SUCCESS == Calculate(exp2, &answer));
	result += (-25 == answer);
	result += (CALC_SYNTAX_ERROR == Calculate(exp3, &answer));
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (-32 == answer);
	result += (CALC_SUCCESS == Calculate(exp5, &answer));
	result += (66 == answer);
	result += (CALC_SYNTAX_ERROR == Calculate(exp6, &answer));
	result += (CALC_SYNTAX_ERROR == Calculate(exp7, &answer));
	
	return (11 == result);
}

int TestErrors()
{	
	double answer = 0;
	int result = 0;
	char *exp1 = "5 5 + +4";
	char *exp2 = "5 * q";
	char *exp3 = "16 - ";
	char *exp4 = "";
	char *exp5 = " ";
	char *exp6 = "52";
	char *exp7 = "2 + -d";
	char *exp8 = "()";
	char *exp9 = "5 +  4";
	char *exp10 = "5 +	4";
	
	result += (CALC_SYNTAX_ERROR == Calculate(exp1, &answer));
	result += (CALC_SYNTAX_ERROR == Calculate(exp2, &answer));
	result += (CALC_SYNTAX_ERROR == Calculate(exp3, &answer));
	result += (CALC_SUCCESS == Calculate(exp4, &answer));
	result += (0 == answer);
	result += (CALC_SYNTAX_ERROR == Calculate(exp5, &answer));
	result += (CALC_SUCCESS == Calculate(exp6, &answer));
	result += (52 == answer);
	result += (CALC_SYNTAX_ERROR == Calculate(exp7, &answer));
	result += (CALC_SYNTAX_ERROR == Calculate(exp8, &answer));
	result += (CALC_SYNTAX_ERROR == Calculate(exp9, &answer));
	result += (CALC_SYNTAX_ERROR == Calculate(exp10, &answer));
	
	return (12 == result);
}
