/***********************
 * Submitted by: Orit. *
 * Reviewed by: Moris. *
 ***********************/ 

#include <stdio.h> /* printf */
#include "bitwise.h"

#define OOOOOIOO 4
#define OOIIOOII 51
#define OIOOOOOO 64
#define OIOIOIOI 85
#define IOIOIOIO 170
#define IIOOIIOO 204
#define OOOOOOOOOOOOOOOOOOOOOOOOIIIIIIII 255
#define OIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOI 1431655765
#define OOIIOOIIOOIIOOIIOOIIOOIIOOIIOOII 858993459
#define OOOOIIIIOOOOIIIIOOOOIIIIOOOOIIII 252645135
#define NUM_BITS_IN_INT 32

long Pow2(unsigned int x, unsigned int y)
{
	return (long)x << y;
}

int IsPowOf2(unsigned int n)
{
	int is_equal = 0;
	unsigned int test = 1;
	
	while(0 != test && 1 != is_equal)
	{
		is_equal = (n == (n & test));
		test <<= 1;
	}
	
	return is_equal;
}

int IsPowOf2ver2(unsigned int n)
{
	return (0 != n && 0 == (n & (n - 1)));
}

unsigned long Add1(unsigned int n)
{
	unsigned long test = 1;
	unsigned long number = (unsigned long)n;
	
	while(0 != (number & test))
	{
		number ^= test;
		test <<= 1;
	}
	number ^= test;
	
	return number;
}

void Is3Bits(unsigned int *array, size_t array_size)
{
	size_t i = 0;
	int counter = 0;
	
	for(i = 0; i < array_size; ++i)
	{
		counter = CountSetBits(*array);
		if(3 == counter)
		{
			printf("%d\n", *array);
		}
		++array;
	}
}

unsigned char ByteMirror1(unsigned char byte)
{
	int i = 0;
	unsigned char mirror_num = 0;
	
	for(i = 0; i < 8; ++i)
	{
		mirror_num <<= 1;
		mirror_num |= (byte >> i) & 1;
	}
	
	return mirror_num;
}

unsigned char ByteMirror2(unsigned char byte)
{ 
	byte = (byte << 4) | (byte >> 4);
	byte = ((byte << 2) & IIOOIIOO) | ((byte >> 2) & OOIIOOII);
	byte = ((byte << 1) & IOIOIOIO) | ((byte >> 1) & OIOIOIOI);

	return byte;
}

int Is2And6(unsigned char byte)
{
	return (byte & OIOOOOOO) && (byte & OOOOOIOO);
}

int Is2Or6(unsigned char byte)
{
	return (byte & OIOOOOOO) || (byte & OOOOOIOO);
}

unsigned char Swap35(unsigned char byte)
{
	unsigned char diff = ((byte >> 5) & 1) ^ ((byte >> 3) & 1);
	
	byte ^= diff << 5;
	byte ^= diff << 3;
	
	return byte;
}

unsigned int Closest16(unsigned int number)
{
	unsigned int lowering_to_closest = ~15;
	
	return number & lowering_to_closest;
}

void Swap(int *num1, int *num2)
{
	if(num1 != num2)
	{
		*num1 ^= *num2;
		*num2 ^= *num1;
		*num1 ^= *num2;
	}
}

int CountSetBits(int n)
{
	int counter = 0;
	
	while(0 != n)
	{
		n = n & (n - 1);
		++counter;
	}
	
	return counter;
}

int CountSetBits2(int n)
{
	n = (n & OIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOI) + 
		((n >> 1) & OIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOI);
	n = (n & OOIIOOIIOOIIOOIIOOIIOOIIOOIIOOII) + 
		((n >> 2) & OOIIOOIIOOIIOOIIOOIIOOIIOOIIOOII);
	n = (n & OOOOIIIIOOOOIIIIOOOOIIIIOOOOIIII) + 
		((n >> 4) & OOOOIIIIOOOOIIIIOOOOIIIIOOOOIIII);
		
	return (n & OOOOOOOOOOOOOOOOOOOOOOOOIIIIIIII) + 
			((n >> 8) & OOOOOOOOOOOOOOOOOOOOOOOOIIIIIIII) + 
			((n >> 16) & OOOOOOOOOOOOOOOOOOOOOOOOIIIIIIII) + 
			((n >> 24) & OOOOOOOOOOOOOOOOOOOOOOOOIIIIIIII);
}

void PrintFloatBits(float number)
{
	unsigned int *float_as_int = (unsigned int *)&number;
	unsigned int test = 1;
	int i = 0;
	
	for(i = 0; i < NUM_BITS_IN_INT; ++i)
	{
		printf("%d", (test >> i) == (*float_as_int & (test >> i)));
	}
	printf("\n");
}
