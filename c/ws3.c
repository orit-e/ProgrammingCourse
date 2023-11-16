#include <stdio.h>  /* printf, size_t, etc...*/
#include <ctype.h>  /* tolower */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen */
#include <assert.h> /* assert*/
#include "ws3.h"

/* Environmet: */
char **Free(char **buffer, const size_t buffer_size)
{
	size_t i = 0;
	
	assert(NULL != buffer);
	assert(0 != buffer_size);
	
	for(i = 0; i < buffer_size; ++i)
	{
		free(*buffer);
		*buffer = NULL;
		++buffer;
	}
	buffer -= buffer_size;
	free(buffer);
	buffer = NULL;
	
	return buffer;
}

size_t CountEnvp(const char **str_envp)
{
	size_t counter = 0;
	
	assert(NULL != str_envp);
	
	do
	{
		++counter;
	}
	while(NULL != *str_envp++)
	
	return counter;
}

char *StrToLower(char *str_buff, const char *str_env)
{
	char *start = str_buff;
	
	assert(NULL != str_buff);
	assert(NULL != str_env);
	
	while('\0' != *str_env)
	{
		*str_buff = (char)tolower(*str_env);
		++str_env;
		++str_buff;
	}
	*str_buff = '\0';
	
	return start;
}

char **InToBuffer(char **buffer, const char **str_envp, const size_t num_envp)
{	
	size_t i = 0;
	size_t str_size = 0;
	char **start_buffer = buffer;
	
	assert(NULL != buffer);
	assert(NULL != str_envp);
	assert(0 != num_envp);
	
	for(i = 0; i < num_envp; ++i)
	{
		str_size = strlen(*str_envp) + 1;
		*buffer = (char *)malloc(sizeof(*buffer) * str_size);
		if(NULL == buffer)
		{
			start_buffer = Free(start_buffer, i);
			
			return start_buffer;
		}
		StrToLower(*buffer, *str_envp);
		++buffer;
		++str_envp;
	}
	
	return start_buffer;
}

void PrintBuffer(const char **buffer, const size_t num_envp)
{
	size_t i = 0;
	
	assert(NULL != buffer);
	assert(0 != num_envp);
	
	for(i = 0; i < num_envp; ++i)
	{
		printf("%s\n", *buffer);
		++buffer;
	}
}

char **EnvToBuffer(const char **str_envp)
{
	const size_t num_envp = CountEnvp(str_envp);
	char **buffer = (char **)malloc(sizeof(*buffer) * num_envp);
	
	assert(NULL != str_envp);
	if(NULL == buffer)
	{
		return NULL;
	}

	InToBuffer(buffer, str_envp, num_envp);
	PrintBuffer((const char **)buffer, num_envp);
	
	buffer = Free(buffer, num_envp);
	
	return buffer;
}

/* Josephouse: */

int *RestartFunc(int *soldiers, const size_t num_soldiers)
{
	size_t i = 0;
	int *start = soldiers;
	
	assert(0 != num_soldiers);
	assert(NULL != soldiers);
	
	for(i = 0; i < (num_soldiers - 1); ++i)
	{
		*soldiers = i + 1;
		++soldiers;
	}
	*soldiers = 0;
	
	return start;
}

size_t JosephousProblem(const size_t num_soldiers)
{
	size_t holder_of_sword = 0;
	size_t next_sword_holder = 0;
	int *soldiers = (int *)malloc(sizeof(*soldiers) * num_soldiers);
	
	assert(0 != num_soldiers);
	
	if(NULL == soldiers)
	{
		return 0;
	}
	
	RestartFunc(soldiers, num_soldiers);
	do
	{
		next_sword_holder = soldiers[holder_of_sword];
		soldiers[holder_of_sword] = soldiers[next_sword_holder];
		holder_of_sword = soldiers[next_sword_holder];
		
	}
	while(soldiers[next_sword_holder] != soldiers[holder_of_sword]);
	
	free(soldiers);
	soldiers = NULL;
	
	return holder_of_sword + 1;
}

/* Data types: */
void PrintZiseDataType()
{
	printf("The size of signed char is:            %lu bytes.\n", 
			sizeof(signed char));
	printf("The size of char is:                   %lu bytes.\n", sizeof(char));
	printf("The size of short int is:              %lu bytes.\n", 
			sizeof(short int));
	printf("The size of unsigned short int is:     %lu bytes.\n", 
			sizeof(unsigned short int));
	printf("The size of int is:                    %lu bytes.\n", sizeof(int));
	printf("The size of unsigned int is:           %lu bytes.\n", 
			sizeof(unsigned int));
	printf("The size of long int is:               %lu bytes.\n", 
			sizeof(long int));
	printf("The size of unsigned long int is:      %lu bytes.\n", 
			sizeof(unsigned long int));
	printf("The size of long long int is:          8 bytes.\n");
	printf("The size of unsigned long long int is: 8 bytes.\n");
	printf("The size of float is:                  %lu bytes.\n", 
			sizeof(float));
	printf("The size of double is:                 %lu bytes.\n", 
			sizeof(double));
	printf("The size of long double is:           %lu bytes,\n", 
			sizeof(long double));
	printf("    But actually it's 10 bytes and the rest is wastefull but\n");
	printf("    usefull for some thigs such as alignment.\n");
}
