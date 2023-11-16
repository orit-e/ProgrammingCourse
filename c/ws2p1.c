#include <stdio.h> /*For printf() that is in main function.*/
#include "ws2p1.h" /*Per request from mentors.*/

void Swap(int *num_1, int *num_2);
void CopyArray(char *ptr_arr, int arr_length, char *ptr_copy);
void SwapUnsignIntegral(size_t *num_1, size_t *num_2);
void SwapUnsignIntegralPtr(size_t **ptr_1, size_t **ptr_2);
int Strlen(const char *str);
int Strcmp(const char *str1, const char *str2);

int main()
{
	int num_1 = 1;
	int num_2 = 2;
	char arr[] = "string!";
	int arr_length = sizeof(arr);
	char arr_copied[80];
	char *ptr_arr = arr;
	char *ptr_copy = arr_copied;
	size_t num_t_1 = 1;
	size_t num_t_2 = 2;
	size_t *ptr_num_t_1 = &num_t_1;
	size_t *ptr_num_t_2 = &num_t_2;
	
	printf("Before int swap: num_1 = %d, num_2 = %d.\n", num_1, num_2);
	Swap(&num_1, &num_2);
	printf("After int swap:  num_1 = %d, num_2 = %d.\n\n", num_1, num_2);
	
	CopyArray(ptr_arr, arr_length, ptr_copy);
	printf("Array to copy: %s\n", arr);
	printf("Array copied:  %s\n\n", arr_copied);
	
	printf("Before size_t swap: num_t_1 = %lu, num_t_2 = %lu.\n", num_t_1, num_t_2);
	SwapUnsignIntegral(&num_t_1, &num_t_2);
	printf("After size_t swap:  num_t_1 = %lu, num_t_2 = %lu.\n\n", num_t_1, num_t_2);	
	
	printf("Before size_t * swap: ptr_num_t_1 is pointing to address: %p\n", (void *)ptr_num_t_1);
	printf("                      ptr_num_t_2 is pointing to address: %p\n", (void *)ptr_num_t_2);
	SwapUnsignIntegralPtr(&ptr_num_t_1, &ptr_num_t_2);
	printf("After size_t * swap:  ptr_num_t_1 is pointing to address: %p\n", (void *)ptr_num_t_1);
	printf("                      ptr_num_t_2 is pointing to address: %p\n", (void *)ptr_num_t_2);
	
	return 0;	
}

void Swap(int *num_1, int *num_2)
{
	int temp = *num_1;

	*num_1 = *num_2;
	*num_2 = temp;
}

void CopyArray(char *ptr_arr, int arr_length, char *ptr_copy)
{
	int i = 0;
	
	for (i = 0; i < arr_length; i++)
	{
		*(ptr_copy + i) = *(ptr_arr + i);
	}
}	

void SwapUnsignIntegral(size_t *num_1, size_t *num_2)
{
	size_t tmp = *num_1;
	
	*num_1 = *num_2;
	*num_2 = tmp;
}

void SwapUnsignIntegralPtr(size_t **ptr_1, size_t **ptr_2)
{
	size_t *tmp = *ptr_1;
	
	*ptr_1 = *ptr_2;
	*ptr_2 = tmp;
}
