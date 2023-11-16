/******************************************************************************
 											
 		filename:			sorts.h			
 		written by:     	Orit			
 		reviewed by:    	comparison sort - Yona, non-comparison sort - Ran
 		last edit:      	03.09.2020		
 											
 ******************************************************************************/
#ifndef __SORTS_H__
#define __SORTS_H__

#include <stddef.h>	/* size_t	*/

void BubbleSort(int *arr, size_t arr_size);

void SelectionSort(int *arr, size_t arr_size);

void InsertionSort(int *arr, size_t arr_size);

void CoutningSort(int *input, int *output, size_t arr_size);

void RadixSort(int *arr, size_t arr_size);

int BinarySearchIter(int *arr, int num_elements, int num);

int BinarySearchRec(int *arr, int num_elements, int num);

int MergeSort(int *arr, size_t num_elements);

void QuickSort(void *base, size_t nmemb, size_t size, 
				int (*compare)(const void *, const void *));

#endif/* __COMPARISON_SORTS_H__ */
