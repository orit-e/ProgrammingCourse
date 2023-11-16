#include <stdio.h>  /*  printf()    */
#include <stdlib.h> /*  rand()      */
#include <time.h>   /*  clock()     */
#include <string.h> /*  memcpy()    */

#include "sorts.h"

#define GREEN_COLOR "\x1b[0;32m"
#define RED_COLOR "\x1b[0;31m"
#define DEFAULT_COLOR "\x1b[0;0m"
#define BLUE_COLOR "\x1b[0;34m"
#define SIZE 5000

enum EXIT_STATUS
{
    FAILED,
    SUCCESS,
    DONE
};

#define RUN_TEST(result, name)     \
    {                              \
        printf("test %s: ", name); \
        switch (result)            \
        {                          \
        case SUCCESS:              \
            printf(GREEN_COLOR);   \
            printf("Success\n");   \
            printf(DEFAULT_COLOR); \
            break;                 \
        case FAILED:               \
            printf(RED_COLOR);     \
            printf("Fail\n");      \
            printf(DEFAULT_COLOR); \
            break;                 \
        case DONE:                 \
            printf(BLUE_COLOR);    \
            printf("Done\n");      \
            printf(DEFAULT_COLOR); \
        };                         \
    }

int TestBubbleSort();
int TestSelectionSort();
int TestInsertionSort();
int TestCoutningSort();
int TestRadixSort();
int TestBinarySearchIter();
int TestBinarySearchRec();
int TestMergeSort();
int TestQuickSort();
static void InitArrIMP();
static int CountComparisons();
static int cmpfunc(const void *a, const void *b);

int q_sort_arr[SIZE] = {0};
int test_arr[SIZE] = {0};
int test_arr_output[SIZE] = {0};
clock_t start, end, total;

int main(int argc, char *argv[])
{

	RUN_TEST(TestBubbleSort(), 		    "TestBubbleSort");
	RUN_TEST(TestSelectionSort(), 	    "TestSelectionSort");
	RUN_TEST(TestInsertionSort(), 	    "TestInsertionSort");
	RUN_TEST(TestCoutningSort(),	    "TestCoutningSort");
	RUN_TEST(TestRadixSort(), 		    "TestRadixSort");
    RUN_TEST(TestBinarySearchIter(), 	"TestBinarySearchIter");
    RUN_TEST(TestBinarySearchRec(), 	"TestBinarySearchRec");
    RUN_TEST(TestMergeSort(), 	        "TestMergeSort");
    RUN_TEST(TestQuickSort(), 	        "TestQuickSort");

    (void)argc;
    (void)argv;

    return 0;
}

int TestBubbleSort()
{
    InitArrIMP(10000);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    BubbleSort(test_arr, SIZE);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Bubble Sort: %ld\n", total);

    return CountComparisons() == SIZE;
}

int TestSelectionSort()
{
    InitArrIMP(100000);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    SelectionSort(test_arr, SIZE);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Selection Sort: %ld\n", total);

    return CountComparisons() == SIZE;
}
int TestInsertionSort()
{

    InitArrIMP(1000000);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    InsertionSort(test_arr, SIZE);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Insertion Sort: %ld\n", total);

    return CountComparisons() == SIZE;
}

int TestCoutningSort()
{

    InitArrIMP(101);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    CoutningSort(test_arr, test_arr_output, SIZE);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Coutning Sort: %ld\n", total);
    memcpy(test_arr, test_arr_output, SIZE * sizeof(int));

    return CountComparisons() == SIZE;
}

int TestRadixSort()
{

    InitArrIMP(10000000);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    RadixSort(test_arr, SIZE);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Radix Sort: %ld\n", total);

    return CountComparisons() == SIZE;
}

int TestBinarySearchIter()
{
    int result = 0;

    result += (100 == 
                BinarySearchIter(test_arr, SIZE, test_arr[100]));
    result += (4997 == 
                BinarySearchIter(test_arr, SIZE, test_arr[4997]));
    result += (-1 == 
                BinarySearchIter(test_arr, SIZE, 10000001));

    return (3 == result);
}

int TestBinarySearchRec()
{
    int result = 0;

    result += (100 == 
                BinarySearchRec(test_arr, SIZE, test_arr[100]));
    result += (4997 == 
                BinarySearchRec(test_arr, SIZE, test_arr[4997]));
    result += (-1 == 
                BinarySearchRec(test_arr, SIZE, 10000001));

    return (3 == result);
}

int TestMergeSort()
{

    InitArrIMP(10000000);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    MergeSort(test_arr, SIZE);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Merge Sort: %ld\n", total);

    return CountComparisons() == SIZE;
}

int TestQuickSort()
{

    InitArrIMP(10000000);
    memcpy(q_sort_arr, test_arr, SIZE * sizeof(int));
    start = clock();
    qsort(q_sort_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU qsort: %ld\n", total);

    start = clock();
    QuickSort(test_arr, SIZE, sizeof(int), cmpfunc);
    end = clock();
    total = end - start;
    printf("Total time taken by CPU Quick Sort: %ld\n", total);

    return CountComparisons() == SIZE;
}

static void InitArrIMP(int limit)
{
    int i = 0;
    for (i = 0; i < SIZE; i++)
    {
        test_arr[i] = rand() % limit;
    }
}

static int CountComparisons()
{
    int res = 0;
    int i = 0;
    for (i = 0; i < SIZE; i++)
    {
        res += (test_arr[i] == q_sort_arr[i]);
    }

    return res;
}

static int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
