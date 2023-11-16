/************************
 *  Submitted by: Orit.	*
 *	Reviewed by: Liron.	*
 ************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */
#include <string.h>	/* strlen */
#include <ctype.h>	/* toupper */
#include "ws10.h"

#define IS_LITTLE_ENDIAN() (((*(short *)"21") & 0xFF) == '2')

enum little_big_endian {LITTLE_ENDIAN = 0, BIG_ENDIAN};

int AtoiBase10(const char *nptr)
{
	long int num = 0;
	int is_minus = 1;
	
	assert(NULL != nptr);
	
	if('-' == *nptr)
	{
		is_minus = -1;
		++nptr;
	}
	while('\0' != *nptr)
	{
		num *= 10;
		num += *nptr - '0';
		++nptr;
	}
	num *= is_minus;
	
	return (int)num;
}

int AtoiUpTo36Base(const char *nptr, int base)
{
	long int num = 0;
	int is_minus = 1;
	int temp = 0;
	
	assert(NULL != nptr);
	assert(1 < base && 37 > base);
	
	if('-' == *nptr)
	{
		is_minus = -1;
		++nptr;
	}
	while('\0' != *nptr)
	{
		num *= base;
		temp = toupper(*nptr);
		num += (('9' < temp) ? temp + 10 - 'A' : temp - '0');
		++nptr;
	}
	num *= is_minus;

	return (int)num;
}

static char *Reverse(char *buffer)
{
	char *buffer_start = buffer;
	char *buffer_end = buffer + strlen(buffer) - 1;
	
	assert(NULL != buffer);
	
	while(buffer_start < buffer_end)
	{
		*buffer_start ^= *buffer_end;
		*buffer_end ^= *buffer_start;
		*buffer_start ^= *buffer_end;
		++buffer_start;
		--buffer_end;
	}
	
	return buffer;
}

char *ItoaBase10(int num, char *buffer)
{
	sprintf(buffer, "%d", num);
	
	return buffer;
}

char *ItoaUpTo36Base(int num, char *buffer, int base)
{
	char *start = buffer;
	long int num_long = num;
	int is_negative = 0;
	int temp = 0;
	
	assert(NULL != buffer);
	assert(1 < base && 37 > base);
	
	if(0 > num)
	{
		is_negative = 1;
		num_long = -num_long;
	}
	do
	{
		temp = num_long % base;
		*buffer = ((9 < temp) ? temp - 10 + 'A' : temp + '0');
		num_long /= base;
		++buffer;
	}
	while(0 != num_long);
	if(1 == is_negative)
	{
		*buffer = '-';
		++buffer;
	}
	*buffer = '\0';
	Reverse(start);
	
	return start;
}

void PrintFirstAndSecondNotThird(const char *arr1, const char *arr2, 
									const char *arr3)
{
	int map_arr[256] = {0};
	unsigned int letter_as_num = 0;
	
	while('\0' != *arr1)
	{
		letter_as_num = *arr1;
		map_arr[letter_as_num] = 1;
		++arr1;
	}
	while('\0' != *arr3)
	{
		letter_as_num = *arr3;
		map_arr[letter_as_num] = 0;
		++arr3;
	}
	while('\0' != *arr2)
	{
		letter_as_num = *arr2;
		if(1 == map_arr[letter_as_num])
		{
			printf("%c ", letter_as_num);
			map_arr[letter_as_num] = 2;
		}
		++arr2;
	}
	printf("\n");
}
	
int IsLittleEndian(void)
{
	int i = 1;
	char *p = (char *)&i;
	
	if(1 == *p)
	{
		return LITTLE_ENDIAN;
	}
	
	return BIG_ENDIAN;
}
