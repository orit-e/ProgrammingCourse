#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/
#include <ctype.h>  /*tolower*/
#include "string.h" /*function declaration*/

size_t Strlen(const char *str)
{
	const char *start = str;
	
	assert(NULL != str);
	
	while('\0' != *str)
	{
		++str;
	}
	
	return (size_t)(str - start);
}

int Strcmp(const char *str1, const char *str2)
{	
	assert(NULL != str1);
	assert(NULL != str2);
	
	while('\0' != *str1 && *str1 == *str2)
	{
		++str1;
		++str2;
	}
	
	return *str1 - *str2;
}

int Strncmp(const char *str1, const char *str2, size_t num)
{	
	size_t i = 0;
	
	assert(NULL != str1);
	assert(NULL != str2);
	
	while(i < num && *str1 == *str2)
	{
		++i;
		if(i == num)
		{
			return *str1 - *str2;
		}
		++str1;
		++str2;
	}
	
	return *str1 - *str2;
}

char *Strcpy(char *destination, const char *source)
{
	char *start = destination;
	
	assert(NULL != destination);
	assert(NULL != source);
	
	while('\0' != *source)
	{
		*destination = *source;
		++destination;
		++source;
	}
	*destination = '\0';
	
	return start;
}

char *Strncpy(char *destination, const char *source, size_t num)
{
	size_t i = 0;
	char *start = destination;
	
	assert(NULL != destination);
	assert(NULL != source);
	
	for(i = 0; '\0' != *source && i < num; ++i)
	{
		*destination = *source;
		++source;
		++destination;
	}
	while(i < num)
	{
		*destination = '\0';
		++destination;
		++i;
	}
	
	return start;
}

int Strcasecmp(const char *str1, const char *str2)
{
	assert(NULL != str1);
	assert(NULL != str2);
	
	while('\0' != *str1 && tolower(*str1) == tolower(*str2))
	{
		++str1;
		++str2;
	}
	
	return *str1 - *str2;
}

char *Strchr(const char *str, int character)
{
	assert(NULL != str);
	
	while('\0' != *str && character != *str)
	{
		++str;
	}
	if('\0' == *str)
	{
		return NULL;
	}
	return (char *)str;
}

char *Strdup(const char *str)
{
	char *str_copy = (char *)malloc(Strlen(str) + 1);
	
	assert(NULL != str);
	
	if(NULL == str_copy)
	{
		return NULL;
	}
	Strcpy(str_copy, str);
	
	return str_copy;
}

char *Strcat(char *destination, const char *source)
{
	int dest_length = Strlen(destination);
	char *dest_end = destination + dest_length;
	
	assert(NULL != destination);
	assert(NULL != source);
	
	Strcpy(dest_end, source);
	
	return destination;
}

char *Strncat(char *destination, const char *source, size_t num)
{
	int dest_length = Strlen(destination);
	char *dest_end = destination + dest_length;
	
	assert(NULL != destination);
	assert(NULL != source);
	
	Strncpy(dest_end, source, num);
	dest_end += num;
	if ('\0' != *dest_end)
	{
		++dest_end;
		*dest_end = '\0';
	}
	
	return destination;
}

char *Strstr(const char *haystack, const char *needle)
{
	size_t needle_length = Strlen(needle);
	
	assert(NULL != haystack);
	assert(NULL != needle);
	
	while('\0' != *haystack && 0 != Strncmp(needle, haystack, needle_length))
	{
		++haystack;
	}
	
	return (char *)haystack;
}	

size_t Strspn(const char *str, const char *accept)
{
	int i = 0;
	int str_length = Strlen(str);
	char *check = NULL;
	size_t counter = 0;
	
	assert(NULL != str);
	assert(NULL != accept);
	
	for (i = 0; i < str_length; ++i)
	{
		check = Strchr(accept, (int)*str);
		if(NULL == check && 0 != counter)
		{
			return counter;
		}
		else if(NULL != check)
		{
			++counter;
		}
		++str;
	}
	
	return counter;
}


/*char *Strtok(char *str, const char *delimeters)
{


}*/
