#ifndef __STRING_H__
#define __STRING_H__

#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/
#include <ctype.h>  /*tolower*/
#include <string.h> /*for tests*/

size_t Strlen(const char *str);
/*
* Description:
* The Strlen() function calculates the length of the string pointed to by str, 
* excluding the terminating null byte ('\0').
*
* Return value:
* The Strlen() function returns the number of bytes in the string pointed to by 
* str.
*/

int Strcmp(const char *str1, const char *str2);
/*
* Description:
* The Strcmp() function compares the two strings str1 and str2. The locale is 
* not taken into account. The comparison is done using unsigned characters.
* Strcmp() returns an integer indicating the result of the comparison, as 
* follows: 0, if the s1 and s2 are equal, a negative value if str1 is less than 
* str2, a positive value if str1 is greater than str2.
*
* Return value:
* The Strcmp() function return an integer less than, equal to, or greater than 
* zero if str1 is found, respectively, to be less than, to match, or be greater 
* than str2.
*/

int Strncmp(const char *str1, const char *str2, size_t num);
/*
* Description:
* The Strncmp() function is similar to the Strcmp() function, except it compares
* only the first (at most) n bytes of str1 and str2.
*
* Return value:
* The Strncmp() function return an integer less than, equal to, or greater than 
* zero if the first n bytes, declered in the variable num, of str1 is found, 
* respectively, to be less than, to match, or be greater than str2.
*/

char *Strcpy(char *destination, const char *source);
/*
* Description:
* The Strcpy() function copies the string pointed to by source, including the
* terminating null byte ('\0'), to the buffer pointed to by destination. The 
* strings may not overlap, and the destination string must be large enough to
* receive the copy. Beware of buffer overruns! (See Bugs.)
*
* Return value:
* The Strcpy() function return a pointer to the destination string.
*
* Notes:
* If the programmer knows (i.e., includes code to test!) that the size of 
* destination is greater than the length of source, then Strcpy() can be used.
*
* Bugs:
* If the destination string of a Strcpy() is not large enough, then anything 
* might happen.
*/

char *Strncpy(char *destination, const char *source, size_t num);
/*
* Desription:
* The Strncpy() function is similar to strcpy() function, except that at most n 
* bytes of source are copied. Warning: If there is no null byte among the first 
* n bytes of source, the string placed in destination will not be null-
* terminated.
* If the length of source is less than n, Strncpy() writes additional null bytes
* to destination to ensure that a total of n bytes are written.
*
* Return value:
* The Strncpy() function return a pointer to the destination string.

* Notes:
* If there is no terminating null byte in the first n bytes of source, Strncpy()
* produces an unterminated string in destination.
*/

int Strcasecmp(const char *str1, const char *str2);
/*
* Description:
* The Strcasecmp() function compares the two strings str1 and str2, ignoring the
* case of the characters. It returns an integer less than, equal to, or greater 
* than zero if s1 is found, respectively, to be less than, to match, or be 
* greater than s2.
*
* Return value:
* The Strcasecmp() function return an integer less than, equal to, or greater 
* than zero if str1 (or the first n bytes thereof) is found, respectively, to be
* less than, to match, or be greater than str2.
*/

char *Strchr(const char *str, int character);
/*
* Description:
* The Strchr() function returns a pointer to the first occurrence of the 
* character, saved in the variable character, in the string str.
* Here "character" means "byte". This function do not work with wide or 
* multibyte characters.
*
* Return value:
* The Strchr() function return a pointer to the matched character or NULL if the
*  character is not found. The terminating null byte is considered part of the 
* string, so that if the variable character is specified as '\0', these 
* functions return a pointer to the terminator.
*/

char *Strdup(const char *str);
/*
* Description:
* The Strdup() function returns a pointer to a new string which is a duplicate 
* of the string str. Memory for the new string is obtained with malloc(3), and 
* can be freed with free(3).
*
* Return value:
* On success, the Strdup() function returns a pointer to the duplicated string. 
* It returns NULL if insufficient memory was available.
*
* Errors:
* Insufficient memory available to allocate duplicate string.
*/

char *Strcat(char *destination, const char *source);
/*
* Description:
* The Strcat() function appends the source string to the destination string, 
* overwriting the terminating null byte ('\0') at the end of destination, and 
* then adds a terminating null byte. The strings may not overlap, and the 
* destination string must have enough space for the result. If destination is 
* not large enough, program behavior is unpredictable.
*
* Return value:
* The Strcat() function return a pointer to the resulting string destination.
*/

char *Strncat(char *destination, const char *source, size_t num);
/*
* Description:
* The Strncat() function is similar to Strcat() function, except that it will 
* use at most n bytes from source, and source does not need to be null-
* terminated if it contains n or more bytes. As with Strcat(), the resulting 
* string in destination is always null-terminated. If source contains n or more 
* bytes, Strncat() writes n+1 bytes to destination (n from source plus the 
* terminating null byte). Therefore, the size of destination must be at least 
* strlen(destination)+n+1.
*
* Return value:
* The Strncat() function return a pointer to the resulting string dest.
*/

char *Strstr(const char *haystack, const char *needle);
/*
* Description:
* The Strstr() function finds the first occurrence of the substring needle in 
* the string haystack. The terminating null bytes ('\0') are not compared.
*/

size_t Strspn(const char *str, const char *accept);
/*
* Description:
* The Strspn() function calculates the length (in bytes) of the initial segment 
* of str which consists entirely of bytes in accept.
*/

/*char *Strtok(char *str, const char *delimeters);*/


#endif /*__STRING_H__*/
