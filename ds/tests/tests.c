#include <stdio.h> /* printf */

#include "tests.h"

#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"
#define BLUE_COLOR		"\x1b[0;34m"

enum EXIT_STATUS {TEST_SUCCESS, TEST_FAILED, TEST_DONE};

void RunTest(int result, char *name)
{
	printf("test %s: ", name);
	switch (result)
	{
		case TEST_SUCCESS:
			printf(GREEN_COLOR);
			printf("Success\n");
			printf(DEFAULT_COLOR);
			break;
/*		case TEST_DONE:
			printf(BLUE_COLOR);
			printf("Done\n");
			printf(DEFAULT_COLOR);
			break;*/
		default:
			printf(RED_COLOR);
			printf("Test failed. Number of errors: %d\n", result);
			printf(DEFAULT_COLOR);
			break;
	};
}