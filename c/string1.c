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
