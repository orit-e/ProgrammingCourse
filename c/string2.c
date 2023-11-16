#include "string.h" /*function declaration*/

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
