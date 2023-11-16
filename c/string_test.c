#include <stdio.h>  /*printf*/
#include <assert.h> /*assert*/
#include <string.h>
#include "string.h"

int main(int argc, char *argv[])
{
	char a[] = "abbfebcdkoolooloomiau";
	char b[10] = "abc";
	char c[] = "abc";
	char d[] = "ABC";
	char e[10] = {0};
	char *f = NULL;
	char g[10] = {0};
	char h = 'f';
	char i[] = "abcABC";
	char j[] = "abcABCab";
	char k[] = {'a', 'b', 'c', '\0', '\0', '\0'};
	char l[] = "bfe";
	char m[] = "zqr";
	size_t n = 0;

/*	Strlen test:*/
	if(21 == Strlen(a))
	{
		printf("Strlen() test success!\n\n");
	}

/*	Strcmp tests:*/
	if(0 == Strcmp(b, c))
	{
		printf("Strcmp test 1 success!\n");
	}
	if(0 != Strcmp(c, d))
	{
		printf("Strcmp test 2 success!\n");
	}
	if(0 != Strcmp(a, b))
	{
		printf("Strcmp test 3 success!\n\n");
	}
	
/*	Strncmp tests:*/
	if(0 == Strncmp(a, b, 2))
	{
		printf("Strncmp test 1 success!\n");
	}
	if(0 == Strncmp(b, c, 4))
	{
		printf("Strncmp test 2 success!\n");
	}
	if(0 != Strncmp(c, d, 3))
	{
		printf("Strncmp test 3 success!\n");
	}
	if(0 != Strncmp(a, b, 4))
	{
		printf("Strncmp test 4 success!\n\n");
	}
	
/*	Strcpy tests:*/
	f = Strcpy(e, b);
	if(0 == Strcmp(e, b))
	{
		printf("Strcpy test 1 success!\n");
	}
	if(0 == Strcmp(f, b))
	{
		printf("Strcpy test 2 success!\n\n");
	}
	
/*	Strncpy tests:*/
	f = Strncpy(g, a, 5);
	if(0 == Strncmp(g, a, 5))
	{
		printf("Strncpy test 1 success!\n");
	}
	if(0 != Strncmp(g, a, 6))
	{
		printf("Strncpy test 2 success!\n");
	}
	if(0 == Strncmp(f, a, 5))
	{
		printf("Strncpy test 3 success!\n");
	}
	f = Strncpy(g, b, 6);
	if(0 == Strncmp(g, b, 4))
	{
		printf("Strncpy test 4 success!\n");
	}
	if(0 == Strncmp(k, g, 6))
	{
		printf("Strncpy test 5 success!\n");
	}
	if(0 == Strncmp(f, b, 4))
	{
		printf("Strncpy test 6 success!\n\n");
	}
	
/*	Strcasecmp tests:*/
	if(0 == Strcasecmp(b, c))
	{
		printf("Strcasecmp test 1 success!\n");
	}
	if(0 == Strcasecmp(c, d))
	{
		printf("Strcasecmp test 2 success!\n");
	}
	if(0 != Strcasecmp(a, b))
	{
		printf("Strcasecmp test 3 success!\n\n");
	}
	
/*	Strchr tests:*/
	f = Strchr(a, h);
	if(&a[3] == f)
	{
		printf("Strchr test 1 success!\n");
	}
	f = Strchr(b, h);
	if(NULL == f)
	{
		printf("Strchr test 2 success!\n\n");
	}
	
/*	Strdup test:*/
	f = Strdup(a);
	if(0 == Strcmp(a, f))
	{
		printf("Strdup test success!\n\n");
	}
	free(f);
	f = NULL;
	
/*	Strcat tests:*/
	f = Strcat(b, d);
	if(0 == Strcmp(b, i))
	{
		printf("Strcat test 1 success!\n");
	}
	if(0 == Strcmp(f, i))
	{
		printf("Strcat test 2 success!\n\n");
	}
	
/*	Strncat tests:*/
	f = Strncat(b, c, 2);
	if(0 == Strcmp(b, j))
	{
		printf("Strncat test 1 success!\n");
	}
	if(0 == Strcmp(f, j))
	{
		printf("Strncat test 2 success!\n\n");
	}

/*	Strstr tests:*/
	f = Strstr(a, l);
	if(&a[2] == f)
	{
		printf("Strstr test 1 success!\n");
	}
	f = Strstr(a, c);
	if('\0' == *f)
	{
		printf("Strstr test 2 success!\n\n");
	}

/*	Strspn tests:*/
	n = Strspn(a, l);
	if(5 == n)
	{
		printf("Strspn test 1 success!\n");
	}
	n = Strspn(a, m);
	if(0 == n)
	{
		printf("Strspn test 2 success!\n\n");
	}
	
	(void)argc;
	(void)argv;	
	
	return 0;
}
