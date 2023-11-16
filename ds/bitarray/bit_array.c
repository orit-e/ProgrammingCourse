/*
 *				filename:			bit_array.c
 *				written by:     	Orit
 *				reviewed by:    	Yona
 *				last edit:      	27.07.2020
 */

#include <stdio.h> 	/* size_t */
#include <assert.h>	/* assert */
#include <limits.h>	/* CHAR_BIT */
#include "bit_array.h"

#define MAX_NUM_IN_WORDSIZE (size_t)(~0)
#define ALL_BITS_OFF (size_t)(0)
#define BITARR_T_SIZE (sizeof(size_t) * CHAR_BIT)
#define WORDSIZE_BYTE_NUM sizeof(size_t)
#define CHAR_MAX_NUM 256
#define TWO_BYTE_IN_BITS (CHAR_BIT * 2)
#define THREE_BYTE_IN_BITS (CHAR_BIT * 3)
#define FOUR_BYTE_IN_BITS (CHAR_BIT * 4)
#define FIVE_BYTE_IN_BITS (CHAR_BIT * 5)
#define SIX_BYTE_IN_BITS (CHAR_BIT * 6)
#define SEVEN_BYTE_IN_BITS (CHAR_BIT * 7)
#define SHIFT_FOUR_BITS 4
#define SHIFT_TWO_BITS 2
#define SHIFT_ONE_BIT 1

bitarr_t BitArraySetAll(bitarr_t arr)
{	
	(void)arr;
	return ~0lu;
}

bitarr_t BitArrayResetAll(bitarr_t arr)
{
	(void)arr;
	return 0;
}

bitarr_t BitArraySetOn(bitarr_t arr, size_t n)
{
	assert(BITARR_T_SIZE > n);
	return (arr | (1lu << n));
}

bitarr_t BitArraySetOff(bitarr_t arr, size_t n)
{
	assert(BITARR_T_SIZE > n);
	
	return (arr & (~(1lu << n)));
}

bitarr_t BitArraySetBit(bitarr_t arr, size_t n, int set_val)
{	
	assert(BITARR_T_SIZE > n);
	assert(2 > set_val);
	
	arr = BitArraySetOff(arr, n);
	
	return (arr | ((bitarr_t)set_val << n));
}

int BitArrayGetVal(bitarr_t arr, size_t n)
{
	assert(BITARR_T_SIZE > n);
	
	return (int)((arr >> n) & 1);
}

bitarr_t BitArrayFlip(bitarr_t arr, size_t n)
{
	assert(BITARR_T_SIZE > n);
	
	return (arr ^ (1lu << n));
}

bitarr_t BitArrayMirror(bitarr_t arr)
{
	arr = (arr << FOUR_BYTE_IN_BITS) | (arr >> FOUR_BYTE_IN_BITS);
	arr = ((arr << TWO_BYTE_IN_BITS) & 0xFFFF0000FFFF0000) |
			((arr >> TWO_BYTE_IN_BITS) & 0x0000FFFF0000FFFF);
	arr = ((arr << CHAR_BIT) & 0xFF00FF00FF00FF00) |
			((arr >> CHAR_BIT) & 0x00FF00FF00FF00FF);
	arr = ((arr << SHIFT_FOUR_BITS) & 0xF0F0F0F0F0F0F0F0) | 
			((arr >> SHIFT_FOUR_BITS) & 0x0F0F0F0F0F0F0F0F);
	arr = ((arr << SHIFT_TWO_BITS) & 0xCCCCCCCCCCCCCCCC) | 
			((arr >> SHIFT_TWO_BITS) & 0x3333333333333333);

	return (((arr << SHIFT_ONE_BIT) & 0xAAAAAAAAAAAAAAAA) | 
			((arr >> SHIFT_ONE_BIT) & 0x5555555555555555));
}

bitarr_t BitArrayRotR(bitarr_t arr, size_t n)
{
	assert(BITARR_T_SIZE > n);
	
	return ((arr >> n) | (arr << (BITARR_T_SIZE - n)));
}

bitarr_t BitArrayRotL(bitarr_t arr, size_t n)
{
	assert(BITARR_T_SIZE > n);
	
	return ((arr << n) | (arr >> (BITARR_T_SIZE - n)));
}

bitarr_t BitArrayCountOn(bitarr_t arr)
{
	arr = (arr & 0x5555555555555555) + 
			((arr >> SHIFT_ONE_BIT) & 0x5555555555555555);
	arr = (arr & 0x3333333333333333) + 
			((arr >> SHIFT_TWO_BITS) & 0x3333333333333333);
	arr = (arr & 0x0F0F0F0F0F0F0F0F) + 
			((arr >> SHIFT_FOUR_BITS) & 0x0F0F0F0F0F0F0F0F);
	
	return (arr & 0x00000000000000FF) + 
			((arr >> CHAR_BIT) & 0x00000000000000FF) +
			((arr >> TWO_BYTE_IN_BITS) & 0x00000000000000FF) +
			((arr >> THREE_BYTE_IN_BITS) & 0x00000000000000FF) +
			((arr >> FOUR_BYTE_IN_BITS) & 0x00000000000000FF) +
			((arr >> FIVE_BYTE_IN_BITS) & 0x00000000000000FF) +
			((arr >> SIX_BYTE_IN_BITS) & 0x00000000000000FF) +
			((arr >> SEVEN_BYTE_IN_BITS) & 0x00000000000000FF);
}

bitarr_t BitArrayCountOff(bitarr_t arr)
{
	return (BITARR_T_SIZE - BitArrayCountOn(arr));
}

char *BitArrayToString(bitarr_t arr, char *str)
{
	char *start = str;
	bitarr_t bit_to_print = BITARR_T_SIZE - 1;
	
	assert(NULL != str);
	
	do
	{
		*str = ((arr >> bit_to_print) & 1) + '0';
		++str;
	}
	while(0 != bit_to_print && bit_to_print--);
	*str = '\0';
	
	return start;
}

/*****************************************************************************/
/* Question 9 functions: */
/* A function to help set bit_arr_mirror_lut */
static unsigned int BitArrayMirrorForLUt(unsigned int byte_arr)
{
	byte_arr = ((byte_arr << SHIFT_FOUR_BITS) & 0xF0F0F0F0) | 
				((byte_arr >> SHIFT_FOUR_BITS) & 0x0F0F0F0F);
	byte_arr = ((byte_arr << SHIFT_TWO_BITS) & 0xCCCCCCCC) | 
				((byte_arr >> SHIFT_TWO_BITS) & 0x33333333);
	byte_arr = ((byte_arr << SHIFT_ONE_BIT) & 0xAAAAAAAA) | 
				((byte_arr >> SHIFT_ONE_BIT) & 0x55555555);
	
	return byte_arr;
}

bitarr_t BitArrayMirrorLut(bitarr_t arr)
{
	static unsigned char bit_arr_mirror_lut[CHAR_MAX_NUM] = {0};
	unsigned char *ptr = (unsigned char *)&arr;
	size_t i = 0;
	
	if(0 == bit_arr_mirror_lut[1])
	{
		for(i = 0; i < CHAR_MAX_NUM; ++i)
		{
			bit_arr_mirror_lut[i] = BitArrayMirrorForLUt((unsigned char)i);
		}
	}
	for(i = 0; i < WORDSIZE_BYTE_NUM; ++i)
	{
		*ptr = bit_arr_mirror_lut[*ptr];
		ptr += 1;
	}
	

	return ((arr << SEVEN_BYTE_IN_BITS) & 0xFF00000000000000) | 
			((arr << FIVE_BYTE_IN_BITS) & 0x00FF000000000000) | 
			((arr << THREE_BYTE_IN_BITS) & 0x0000FF0000000000) | 
			((arr << CHAR_BIT) & 0x000000FF00000000) | 
			((arr >> CHAR_BIT) & 0x00000000FF000000) | 
			((arr >> THREE_BYTE_IN_BITS) & 0x0000000000FF0000) | 
			((arr >> FIVE_BYTE_IN_BITS) & 0x000000000000FF00) | 
			((arr >> SEVEN_BYTE_IN_BITS) & 0x00000000000000FF);
}

/* A function to help set bit_arr_count_on_lut */
static unsigned char BitArrayForCountOnLut(unsigned char byte_arr)
{
	byte_arr = (byte_arr & 0x55) + ((byte_arr >> SHIFT_ONE_BIT) & 0x55);
	byte_arr = (byte_arr & 0x33) + ((byte_arr >> SHIFT_TWO_BITS) & 0x33);
	
	return byte_arr = (byte_arr & 0x0F) + 
			((byte_arr >> SHIFT_FOUR_BITS) & 0x0F);
}

bitarr_t BitArrayCountOnLut(bitarr_t arr)
{
	static unsigned char bit_arr_count_on_lut[CHAR_MAX_NUM] = {0};
	unsigned char *ptr = (unsigned char *)&arr;
	size_t i = 0;
	
	if(0 == bit_arr_count_on_lut[1])
	{
		for(i = 0; i < CHAR_MAX_NUM; ++i)
		{
			bit_arr_count_on_lut[i] = BitArrayForCountOnLut((unsigned char)i);
		}
	}
	for(i = 0; i < WORDSIZE_BYTE_NUM; ++i)
	{
		*ptr = bit_arr_count_on_lut[*ptr];
		ptr += 1;
	}
	
	return arr + (arr >> CHAR_BIT) + (arr >> TWO_BYTE_IN_BITS) + 
			(arr >> THREE_BYTE_IN_BITS) + (arr >> FOUR_BYTE_IN_BITS) + 
			(arr >> FIVE_BYTE_IN_BITS) + (arr >> SIX_BYTE_IN_BITS) + 
			(arr >> SEVEN_BYTE_IN_BITS);
}
