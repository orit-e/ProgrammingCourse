#ifndef __BITWISE_H__
#define __BITWISE_H__

/* DESCRIPTION:
 * Calculate (x * 2^y)
 *
 * @param:
 * x, y 	any unsigned integers 
 *
 * @return:
 * return the calculation result
 */
long Pow2(unsigned int x, unsigned int y);

/* DESCRIPTION:
 * Checks whether a number is a power of 2
 *
 * @param:
 * n 		any unsigned integer 
 *
 * @return:
 * return 1 if n is a power of 2; otherwise return 0.
 */
int IsPowOf2(unsigned int n);
int IsPowOf2ver2(unsigned int n);

/* DESCRIPTION:
 * Add 1 to a given number
 *
 * @param:
 * n	 	any unsigned integer 
 *
 * @return:
 * return (n + 1)
 */
unsigned long Add1(unsigned int n);

/* DESCRIPTION:
 * receives array of unsigned ints and prints only the ints that
 * have exactly 3 bits on (set to '1').
 *
 * @param:
 * arr		array of any unsigned integers
 * len		length of arr
 */
void Is3Bits(unsigned int *arr, size_t len);

/* DESCRIPTION:
 * Mirroring given char (for example: 11110000 => 00001111).
 *
 * @param:
 * c		any unsigned char 
 *
 * @return:
 * return the mirroring char
 */
unsigned char ByteMirror1(unsigned char c);
unsigned char ByteMirror2(unsigned char c);

/* DESCRIPTION:
 * Checks if bits 2 and 6 are on ( 0X000X00 )
 *
 * @param:
 * c		any unsigned char 
 *
 * @return:
 * return 1 if bits 2 and 6 are both '1', else return 0.
 */
int Is2And6(unsigned char c);

/* DESCRIPTION:
 * Checks if bits 2 or 6 are on ( 0X000X00 )
 *
 * @param:
 * c		any unsigned char 
 *
 * @return:
 * return 1 if bits 2 or 6 are both '1', else return 0.
 */
int Is2Or6(unsigned char c);

/* DESCRIPTION:
 * Swap bit 3 and bit 5 of a given char (for example: 00001000 => 00100000)
 *
 * @param:
 * c		any unsigned char 
 *
 * @return:
 * the result char
 */
unsigned char Swap35(unsigned char c);

/* DESCRIPTION:
 * return the closest smaller number that is divisible by 16 without a remainder
 *
 * @param:
 * n		any unsigned integer 
 *
 * @return:
 * the result number
 */
unsigned int Closest16(unsigned int n);

/* DESCRIPTION:
 * swap to given numbers
 *
 * @param:
 * n1, n2	any integers 
 *
 */
void Swap(int *n1, int *n2);

/* DESCRIPTION:
 * count the numbers of 'on' bits in a given integer
 *
 * @param:
 * n		any integer 
 *
 * @return:
 * return the result
 */
int CountSetBits(int n);
int CountSetBits2(int n);

/* DESCRIPTION:
 * Prints the bit representation of a given float number
 *
 * @param:
 * f		any float number 
 */
void PrintFloatBits(float f);

#endif /* __BITWISE_H__ */
