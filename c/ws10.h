/************************
 *  Submitted by: Orit.	*
 *	Reviewed by: Liron.	*
 ************************/

#ifndef __WS10_H__
#define __WS10_H__

/* DESCRIPTION:
 * The AtoiBase10() function converts string pointed at by nptr to an integer 
 * number. The function works for base 10 conversion only.
 *
 * Return value:
 * The AtoiBase10() function returns the converted sring as integer number.
 */
int AtoiBase10(const char *nptr);

/* DESCRIPTION:
 * The AtoiUpTo36Base() function converts string pointed at by nptr to an 
 * integer number. The function works for the (inclucive) base range of 2 - 36.
 *
 * Return value:
 * The AtoiUpTo36Base() function returns the converted sring as integer number.
 */
int AtoiUpTo36Base(const char *nptr, int base);

/* DESCRIPTION:
 * The ItoaBase10() function converts num integer number to string 
 * representation and saves it in char variable pointed to by nptr. The function
 * works for base 10 conversion only.
 *
 * Return value:
 * The ItoaBase10() function returns a pointer to nptr.
 */
char *ItoaBase10(int num, char *str);

/* DESCRIPTION:
 * The ItoaUpTo36Base() function converts num integer number to string 
 * representation and saves it in char variable pointed to by nptr. The function
 * works for the (inclucive) base range of 2 - 36.
 *
 * Return value:
 * The ItoaUpTo36Base() function returns a pointer to nptr.
 */
char *ItoaUpTo36Base(int num, char *str, int base);

/* DESCRIPTION:
 * The PrintFirstAndSecondNotThird() function compares 3 strings pointed at by 
 * arr1, arr2 and arr3, and prints out characters that exist in both arr1 and
 * arr2 but not in arr3.
 */
void PrintFirstAndSecondNotThird(const char *arr1, 
								const char *arr2, 
								const char *arr3);

/* DESCRIPTION:
 * The IsLittleEndian() function test whether memory is being stored in little 
 * endian order or not.
 *
 * Return value:
 * The IsLittleEndian() function returns LITTLE_ENDIAN(0) if yes or 
 * BIG_ENDIAN(1) if not.
 */
int IsLittleEndian(void);

#endif /* __WS10_H__ */
