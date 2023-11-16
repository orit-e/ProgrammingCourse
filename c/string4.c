#include "string.h" /*function declaration*/

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
