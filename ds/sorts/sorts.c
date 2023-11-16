/******************************************************************************
 											
 		filename:			sorts.c			
 		written by:     	Orit			
 		reviewed by:    	comparison sort - Yona, non-comparison sort - Ran
		 					third part - Renee
 		last edit:      	04.10.2020		
 											
 ******************************************************************************/
#include <assert.h>	/* assert()			*/
#include <string.h>	/* memset()			*/
#include <stdlib.h>	/* malloc(), free()	*/

#include "sorts.h"

/******************************************************************************/
/*									defines:								  */
#define MAX_DEC_DIGITS 10
#define COUNTER_MAX_NUM 101

/******************************************************************************/
/*		        	Static functions declarations for part:		       	  	  */
static void Swap(int *num1, int *num2);
static void RadixHelper(int *input, int *output, size_t arr_size, int pow);
static int MaxIntInArr(int *arr, size_t arr_size);
static void PointerSwap(int **ptr1, int **ptr2);
static int Merge(int *arr, int num_elements, int mid);
static void VoidSwap(char *ptr1, char *ptr2, size_t size);
static size_t Partition(void *base, size_t nmemb, size_t size, 
						int (*compare)(const void *, const void *));

/******************************************************************************/
/*							Sorts part 1 functions:							  */
void BubbleSort(int *arr, size_t arr_size)
{
	size_t i = 0;
	size_t j = 0;
	int stop_flag = 1;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	for(i = 0; i < arr_size && 1 == stop_flag; ++i)
	{
		stop_flag = 0;
		for(j = 0; j < (arr_size - i - 1); ++j)
		{
			if(arr[j] > arr[j + 1])
			{
				Swap(&arr[j], &arr[j + 1]);
				stop_flag = 1;
			}
		}
	}
}
   
void SelectionSort(int *arr, size_t arr_size)
{
	size_t i = 0;
	size_t j = 0;
	size_t max = 0;
	int stop_flag = 1;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	for(i = 0; i < arr_size && 1 == stop_flag; ++i)
	{
		stop_flag = 0;
		max = i;
		for(j = 0; j <= i; ++j)
		{
			if(arr[j] >= arr[max])
			{
				max = j;
				Swap(&arr[i], &arr[max]);
				stop_flag = 1;
			}
		}
	}
}

void InsertionSort(int *arr, size_t arr_size)
{
	size_t i = 0;
	size_t j = 0;
	int index = 0;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	for(i = 1; i < arr_size; ++i)
	{
		index = arr[i];
		j = i;
		while(j > 0 && arr[j - 1] > index)
		{
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = index;
	}
}

/******************************************************************************/
/*							Sorts part 2 functions:							  */
void CoutningSort(int *input, int *output, size_t arr_size)
{
	size_t i = 0;
	int counter[COUNTER_MAX_NUM] = {0};
	
	assert(NULL != input);
	assert(NULL != output);
	assert(0 != arr_size);

	for(i = 0; i < arr_size; ++i)
	{
		counter[input[i]] += 1;
	}
	for(i = 1; i < COUNTER_MAX_NUM; ++i)
	{
		counter[i] += counter[i - 1];
	}
	for(i = 1; i <= arr_size; ++i)
	{
		output[counter[input[arr_size - i]] - 1] = input[arr_size - i];
		counter[input[arr_size - i]] -= 1;
	}
}

void RadixSort(int *arr, size_t arr_size)
{
	int max = 0;
	int pow = 1;
	int *input = NULL;
	int *temp_arr = NULL;
	int *output = NULL;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	input = arr;
	temp_arr = (int *)malloc(sizeof(int) * arr_size);
	if(NULL == temp_arr)
	{
		return;
	}
	output = temp_arr;
	memset(temp_arr, 0, sizeof(int) * arr_size);
	max = MaxIntInArr(arr, arr_size);
	for(pow = 1; max / pow > 0; pow *= MAX_DEC_DIGITS)
	{
		 RadixHelper(input, output, arr_size, pow);
		 PointerSwap(&input, &output);
	}
	memcpy(arr, input, arr_size * sizeof(int));
	free(temp_arr);
}
/******************************************************************************/
/*							Sorts part 3 functions:							  */
int BinarySearchIter(int *arr, int num_elements, int num)
{
	int low = 0;
	int high = num_elements - 1;
	int index = -1;

	assert(NULL != arr);
	
	while(high >= low)
	{
		int mid = (high + low) / 2;
		int cmp = arr[mid] - num;

		if(0 == cmp)
		{
			index = mid;
			high = low - 1;
		}
		else if(0 < cmp)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	return index;
}

int BinarySearchRec(int *arr, int num_elements, int num)
{
	int mid = (num_elements - 1) / 2;
	int cmp = 0;

	assert(NULL != arr);

	if(0 >= num_elements)
	{
		return -1;
	}

	cmp = arr[mid] - num;
	if(0 == cmp)
	{
		return mid;
	}
	if(0 < cmp)
	{
		return BinarySearchRec(arr, mid, num);
	}
	else
	{
		int index = BinarySearchRec(arr + mid + 1, num_elements - mid - 1, num);
		
		return ((-1 == index) ? -1 : index + mid + 1);
	}
}

int MergeSort(int *arr, size_t num_elements)
{
	int mid = num_elements / 2;
	int status = 0;

	assert(NULL != arr);

	if(2 > num_elements)
	{
		return status;
	}
	status = MergeSort(arr, mid);
	if(0 != status)
	{
		return status;
	}
	status = MergeSort(arr + mid, (num_elements - mid));
	if(0 != status)
	{
		return status;
	}

	return Merge(arr, num_elements, mid);
}

void QuickSort(void *base, size_t nmemb, size_t size, 
				int (*compare)(const void *, const void *))
{
	size_t pivot = 0;

	assert(NULL != base);
	assert(0 < size);
	assert(NULL != compare);
	
	pivot = Partition(base, nmemb, size, compare);
	if(2 < nmemb && 0 == pivot)
	{
		pivot = nmemb / 2;
	}
	
	if(2 < nmemb)
	{
		QuickSort(base, pivot, size, compare);
		QuickSort((void *)((char *)base + pivot * size), 
					nmemb - pivot, size, compare);
	}
	
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
static void Swap(int *num1, int *num2)
{
	int temp = 0;
	
	assert(NULL != num1);
	assert(NULL != num2);
	
	temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

static void RadixHelper(int *input, int *output, size_t arr_size, int pow)
{
	size_t i = 0;
	int counter[MAX_DEC_DIGITS] = {0};
	int index = 0;
	
	assert(NULL != input);
	assert(NULL != output);
	assert(0 != arr_size);
	assert(0 < pow);

	for(i = 0; i < arr_size; ++i)
	{
		index = (input[i] / pow) % MAX_DEC_DIGITS;
		counter[index] += 1;
	}
	for(i = 1; i < MAX_DEC_DIGITS; ++i)
	{
		counter[i] += counter[i - 1];
	}
	for(i = 1; i <= arr_size; ++i)
	{
		index = (input[arr_size - i] / pow) % MAX_DEC_DIGITS;
		output[counter[index] - 1] = input[arr_size - i];
		counter[index] -= 1;
	}
}

static int MaxIntInArr(int *arr, size_t arr_size)
{
	size_t i = 0;
	int max = 0;
	
	assert(NULL != arr);
	assert(0 != arr_size);
	
	max = arr[0];
	for(i = 1; i < arr_size; ++i)
	{
		if(arr[i] > max)
		{
			max = arr[i];
		}
	}
	
	return max;
}

static void PointerSwap(int **ptr1, int **ptr2)
{
	int *temp = 0;
	
	assert(NULL != ptr1);
	assert(NULL != ptr2);
	
	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}

static int Merge(int *arr, int num_elements, int mid)
{
	int i = 0;
	int j = mid;
	int k = 0;
	int *temp = NULL;

	assert(NULL != arr);

	temp = (int *)malloc(sizeof(int) * num_elements);
	if(NULL == temp)
	{
		return 1;
	}
	memset(temp, 0, sizeof(int) * num_elements);

	for(k = 0; k < num_elements; ++k)
	{
		temp[k] = arr[k];
	}
	for(k = 0; i < mid && j < num_elements; ++k)
	{
		if(temp[i] <= temp[j])
		{
			arr[k] = temp[i];
			++i;
		}
		else
		{
			arr[k] = temp[j];
			++j;
		}
	}
	for(;i < mid; ++i, ++k)
	{
		arr[k] = temp[i];
	}
	for(;j < num_elements; ++j, ++k)
	{
		arr[k] = temp[j];
	}

	free(temp);

	return 0;
}

static void VoidSwap(char *ptr1, char *ptr2, size_t size)
{
	size_t i = 0;
	char temp = 0;

	assert(NULL != ptr1);
	assert(NULL != ptr2);

	for(i = 0; i < size; ++i, ++ptr1, ++ptr2)
	{
		temp = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = temp;
	}
}

static size_t Partition(void *base, size_t nmemb, size_t size, 
						int (*compare)(const void *, const void *))
{
	void *pivot = 0;
	char *i = (char *)base;
	char *j = (char *)base;

	assert(NULL != base);
	assert(NULL != compare);

	pivot = (void *)((char *)base + (nmemb - 1) * size);
	while((size_t)i < (size_t)pivot)
	{
		if(0 > compare((const void *)i, (const void *)pivot))
		{
			VoidSwap(i, j, size);
			j += size;
		}
		i += size;
	}
	
	VoidSwap(j, (char *)pivot, size);

	return ((size_t)j - (size_t)base) / size;
}
