#include <stdio.h>  /* printf, size_t, etc...*/
#include <ctype.h>  /* tolower */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen */
#include "ws3.h"

int main(int argc, char *argv[], char *envp[])
{
	size_t num_soldiers = 1;
    size_t res = 0;
	size_t i = 0;
	size_t last = 0;

/*	Environmet test: */	
	EnvToBuffer((const char **)envp);
	
/*	Josephouse test: */
	for(num_soldiers = 1; num_soldiers < 10; ++num_soldiers)
	{
		last = JosephousProblem(num_soldiers);
    	for (i = 1; i <= num_soldiers; ++i)
    	{
    		res = (res + 2) % i;
		}
		if(res + 1 == last)
		{
			printf("Succes for %lu soldiers!\n", num_soldiers);
		}
		res = 0;
	}
	
/*	Data types test: */
	PrintZiseDataType();
	
	(void)argc;
	(void)argv;
	(void)envp;
	
	return 0;
}
