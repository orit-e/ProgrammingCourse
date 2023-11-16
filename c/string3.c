#include "string.h" /*function declaration*/

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
