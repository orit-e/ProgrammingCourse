#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "knights_tour.h"
#include "tests.h"

int TestNaive();
int TestNotNaive();



int main(int argc, char *argv[])
{
	RunTest(TestNaive(),			"TestNaive");
	RunTest(TestNotNaive(),			"TestNotNaive");


	(void) argc;
	(void) argv;

    return 0;
}

int TestNaive()
{
    char start[] = "1A";
	
	KnightsTourNaive(start);
	printf("\n");
	start[1] = 'F';
	KnightsTourNaive(start);
	printf("\n");
	start[0] = '5';
	start[1] = 'D';
	KnightsTourNaive(start);
	
	return 0;
}

int TestNotNaive()
{
	char start[] = "1A";
	KnightsTour(start);
	printf("\n");
	start[1] = 'F';
	KnightsTour(start);
	printf("\n");
	start[0] = '5';
	start[1] = 'D';
	KnightsTour(start);
	
	return 0;
}
