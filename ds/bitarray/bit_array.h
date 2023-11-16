/*
 *				filename:			bit_array.h
 *				written by:     	Orit
 *				reviewed by:    	Yona
 *				last edit:      	27.07.2020
 */

#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>

typedef size_t bitarr_t;

/* DESCRIPTION:
 * Set all bits in the arr on.
 *
 * @param:
 * arr			word size bit-array.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArraySetAll(bitarr_t arr);

/* DESCRIPTION:
 * Reset all bits in the bit-array back to off.
 *
 * @param:
 * arr			word size bit-array.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArrayResetAll(bitarr_t arr);

/* DESCRIPTION:
 * Set the n-th bit in the bit-array on.
 *
 * @param:
 * arr			word size bit-array.
 * n			bit number to set on.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArraySetOn(bitarr_t arr, size_t n);

/* DESCRIPTION:
 * Set the n-th bit in the bit-array off.
 *
 * @param:
 * arr          word size bit-array.
 * n            bit number to set off.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArraySetOff(bitarr_t arr, size_t n);

/* DESCRIPTION:
 * Set the n-th bit in the bit-array on or off.
 *
 * @param:
 * arr          word size bit-array.
 * n			bit number to set on or off.
 * set_val		turn desired bit on or off.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArraySetBit(bitarr_t arr, size_t n, int set_val);

/* DESCRIPTION:
 * get the value of the n-th bit in the bit-array.
 *
 * @param:
 * arr          word size bit-array.
 * n			bit number to get value of.
 *
 * @return:
 * value of the desired bit mark. (0 for off and 1 for on.)
 */
int BitArrayGetVal(bitarr_t arr, size_t n);

/* DESCRIPTION:
 * flip the value of the n-th bit in the bit-array.
 *
 * @param:
 * arr          word size bit-array.
 * n			bit number to flip.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArrayFlip(bitarr_t arr, size_t n);

/* DESCRIPTION:
 * mirrors bit values of all bits in the bit-array.
 *
 * @param:
 * arr          word size bit-array.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArrayMirror(bitarr_t arr);

/* DESCRIPTION:
 * rotate bit-array n times to the right.
 *
 * @param:
 * arr          word size bit-array.
 * n			number of right rotations.
 *
 * @return:
 * the rotated array.
 */
bitarr_t BitArrayRotR(bitarr_t arr, size_t n);

/* DESCRIPTION:
 * rotate bit-array n times to the left.
 *
 * @param:
 * arr          word size bit-array.
 * n			number of left rotations.
 *
 * @return:
 * the rotated array.
 */
bitarr_t BitArrayRotL(bitarr_t arr, size_t n);

/* DESCRIPTION:
 * counts the number of on bits in the bit-array.
 *
 * @param:
 * arr          word size bit-array.
 *
 * @return:
 * number of on bits.
 */
bitarr_t BitArrayCountOn(bitarr_t arr);

/* DESCRIPTION:
 * counts the number of off bits in the bit-array.
 *
 * @param:
 * arr          word size bit-array.
 *
 * @return:
 * number of off bits.
 */
bitarr_t BitArrayCountOff(bitarr_t arr);

/* DESCRIPTION:
 * converts word size bit-array to string representation.
 *
 * @param:
 * arr          word size bit-array.
 * str			buffer where the string will be stored.
 *
 * @return:
 * string representation.
 */
char *BitArrayToString(bitarr_t arr, char *str);

/*****************************************************************************/
/* Question 9 functions: */

/* DESCRIPTION:
 * same as BitArrayMirror() function, now implemented with LUT.
 *
 * @param:
 * arr          word size bit-array.
 *
 * @return:
 * the modified array.
 */
bitarr_t BitArrayMirrorLut(bitarr_t arr);

/* DESCRIPTION:
 * same as BitArrayCountOn() function, now implemented with LUT.
 *
 * @param:
 * arr          word size bit-array.
 *
 * @return:
 * number of off bits.
 */
bitarr_t BitArrayCountOnLut(bitarr_t arr);

#endif /* __BIT_ARRAY_H__ */
