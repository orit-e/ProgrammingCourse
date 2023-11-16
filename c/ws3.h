#ifndef __WS3_H__
#define __WS3_H__

#include <stdio.h>  /* printf, size_t, etc...*/
#include <ctype.h>  /* tolower */
#include <stdlib.h> /* malloc */

/* Environmet: */
char **Free(char **buffer, const size_t buffer_size);
/*
* Description:
* The Free() function free all allocated space pointed to by each of the 
* pointers in the buffer pointer array, and finally frees the allocated space 
* pointed to by the buffer array itself.
*/

size_t CountEnvp(const char **str_envp);
/*
* Description:
* The CountEnvp() function takes the str_envp pointer array and counts how many 
* pointers are stored in it. Than the function return that number.
*/

char *StrToLower(char *str_buff, const char *str_env);
/*
* Description:
* The StrToLower() function takes the char signs in str_env and copy them in to 
* str_buff in lower letters.
*/

char **InToBuffer(char **buffer, const char **str_envp, const size_t num_envp);
/*
* Description:
* The InToBuffer() function measurs the length of each environment pointed to by 
* one of the pointers in the str_envp pointer array, creates a new string on the
* heap accordingly that is pointed to by the appropriate pointer in the buffer
* pointer array  and copies the environment to it with lower letters only.
*/

void PrintBuffer(const char **buffer, size_t num_envp);
/*
* Dscription:
* The PrintBuffer() function print out all of the strings that the pointers in 
* buffer pointer array points to.
*/

char **EnvToBuffer(const char **str_envp);
/*
* Description:
* The EnvToBuffer() takes envp from the main function through the str_envp 
* pointer array, stores all of the environments pointed to by str_envp in new 
* arrays on the heap in lower letters only. During this proccess EnvToBuffer 
* creats a new buffer array of char ** type with pointers that points at the 
* new storing place of the environments. At the end of this proccess the 
* function prints out all of the environments in lower letters and finally free 
* all of the arrays that were created from the heap.
*/

/* Josephouse: */
int *RestartFunc(int *soldiers, const size_t num_soldiers);
/*
* Description:
* The RestartFunc() functoin places the integer of the next element in the array
* in all of the elements of the soldiers array using num_soldiers to determining
* it's size. The last element of the soldiers array recieves the integer 0 
* representing the first element of the soldiers array. This action makes the 
* linear array act as though circular. At the end of this process, the 
* RestartFunc() returns the beginning address of soldiers array.
*/

size_t JosephousProblem(const size_t num_soldiers);
/*
* Description:
* The JosephousProblem() function receives the total number of soldiers that 
* are standing in the circle and returns the number of the soldier that remains
* alive at the end. The function works for positive integers only.
*/

/* Data types: */
void PrintZiseDataType();
/*
* Description:
* The PrintZiseDataType() function, when called, prints out the sizes of all 
* data types in c.
*/

#endif /*__WS3_H__*/
