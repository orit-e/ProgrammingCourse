/******************************************************************************
 											
				File name:			heap.h			
				Written by:     	Orit			
				Reviewed by:    	Yona
 											
 ******************************************************************************/
#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* size_t */

/** 
 * @DESCRIPTION:
 * heap_cmp_func_ty comparison function pointer is given by the user at 
 * HeapCreate() and is used by heap functions to sort and find inserted data. 
 * The comparison function will receive and compare data1 and data2 with the 
 * optional help of param, also given by user at HeapCreate(), and will determin
 * their order.
 * heap_cmp_func_ty comparison function pointer will return an integer 
 * indicating the result of the comparison as follows:
 * *0 if the data1 and data2 comparison keys are equal.
 * *a negative value if data1 comparison key is of lesser priority than data2 
 *  comparison key.
 * *a positive value if data1 comparison key is of greater priority than data2 
 *  comparison key.
 * The user can choose whether the heap will be a max_heap or a min_heap with
 * the result given by the comparison function (see EXAMPLE section for further 
 * details).
 *
 * @RETURN:
 * heap_cmp_func_ty comparison function pointer will return an integer less 
 * than, equal to, or greater than zero if data1 comparison key is found, 
 * respectively, to be of lesser, equal, or of greater priority than data2 
 * comparison key.
 * 
 * @EXAMPLE:
 * For max_heap:
 * int TestCompareFunc1(const void *data1, const void *data2, 
 * 							void *param)
 * {    
 *     (void)param;    
 *     
 *     return *(int*)data1 - *(int*)data2;
 * }
 * 
 * For min_heap:
 * int TestCompareFunc2(const void *data1, const void *data2, 
 * 							void *param)
 * {    
 *     (void)param;    
 *     
 *     return *(int*)data2 - *(int*)data1;    
 * }  
 */
typedef int (*heap_cmp_func_ty)(const void *data1, const void *data2,
                                                        void *param);

/** 
 * @DESCRIPTION:
 * heap_match_func_ty matching function pointer is given by the user at 
 * HeapRemove() and is used to find the data to be removed. The function 
 * will determine whether some parameter of data is equal to the searched 
 * match_param or not and wiil return the appropriate boolean value accordingly.
 * 
 * @RETURN:
 * heap_match_func_ty matching function is expected to return 1 if data is 
 * matched to match_param, and 0 otherwise. Any other values returned by this 
 * function will result in undefined behavior.
 */
typedef int (*heap_match_func_ty)(void *data, void *match_param);
typedef struct heap heap_ty;

/**
 * @DESCRIPTION:
 * HeapCreate() function creates a new heap. The function receives a compare 
 * function pointer and a param from the user for with which the heap will be 
 * heap-ordered (see heap_cmp_func_ty for compare function signature 
 * specifications and also see HeapPush() and HeapPop() for further 
 * information).
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * compare			pointer to comparison function.
 * param			helping parameter for compare.
 *
 * @RETURN:
 * HeapCreate() function returns a pointer to a new and empty heap.
 * In case of malloc failure, NULL will be returned.
 *
 * @ERROR:
 * If malloc fails, the function will return NULL.
 * 
 * @WARNING:
 * Do not place NULL as input of compare. Such a thing will lead to undefined 
 * behavior.
 */
heap_ty *HeapCreate(heap_cmp_func_ty compare, void *param);

/**
 * @DESCRIPTION:
 * HeapDestroy() function destroys given heap structure.
 * The function performs with time complexity of O(n) where n is the number of 
 * data elements in the heap.
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 *
 * @WARNING:
 * Do not place NULL as input of heap. Such a thing will lead to undefined 
 * behavior.
 */
void HeapDestroy(heap_ty *heap);

/**
 * @DESCRIPTION:
 * HeapPush() function receives data to place in the heap and then, with the 
 * help of the compare function and param given in HeapCreate(), select the 
 * right place in the heap for it to be inserted.
 * The function performs with amortized time complexity of O(logn) and worst 
 * case time complexity of O(n) where n is the number of data elements already 
 * in the heap.
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 * data				data to be pushed in to the heap.
 *
 * @RETURN:
 * HeapPush() function return 0 for success and 1 otherwise.
 *
 * @WARNINGS:
 * *Do not place NULL as input of heap or data. Such a thing will lead to 
 *  undefined behavior.
 * *Function failure is an indication that realloc() function has failed to 
 *  reallocate the heap (see realloc() man page for further details).
 */
int HeapPush(heap_ty *heap, void *data);

/**
 * @DESCRIPTION:
 * HeapPop() function removes from the heap the data with the highest priority, 
 * as was prioritized by compare function and param given in HeapCreate() and 
 * replaces it with the next highest prioritized data in its place. 
 * The function performs with time complexity of O(n) where n is the number of 
 * data elements already in the heap.
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 *
 * @WARNINGS:
 * *Do not place NULL as input of heap. Such a thing will lead to undefined 
 *  behavior.
 * *Trying to remove data from an empty heap will lead to undefined behavior.
 */
void HeapPop(heap_ty *heap);

/**
 * @DESCRIPTION:
 * HeapPeek() function returns to the user a peek at the data with the highest 
 * priority, as was prioritized by compare function and param given in 
 * HeapCreate().
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 *
 * @WARNINGS:
 * *Do not place NULL as input of heap. Such a thing will lead to undefined 
 *  behavior.
 * *Trying to peak data from an empty heap will lead to undefined behavior.
 */
void *HeapPeek(const heap_ty *heap);

/**
 * @DESCRIPTION:
 * HeapSize() function returns the number of data elements in given heap.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 *
 * @RETURN:
 * HeapSize() function returns the number of data elemnts in given heap.
 *
 * @WARNING:
 * Do not place NULL as input of heap. Such a thing will lead to undefined 
 * behavior.
 */
size_t HeapSize(const heap_ty *heap);

/**
 * @DESCRIPTION:
 * HeapIsEmpty() function checks whether given heap is empty or not.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 *
 * @RETURN:
 * HeapIsEmpty() function will return 1 if heap is empty, otherwise 0.
 *
 * @WARNING:
 * Do not place NULL as input of heap. Such a thing will lead to undefined 
 * behavior.
 */
int HeapIsEmpty(const heap_ty *heap);

/**
 * @DESCRIPTION:
 * HeapRemove() function, with the help of is_match function, searches for data 
 * matching match_param (see heap_match_func_ty matching function signature 
 * specifications for further details). If a match is found, the function will 
 * remove it from the heap and return the removed data to the user. If there is 
 * more than one potential match to match_param, the function will only remove 
 * the first match that is found and will return to the user the removed data.
 * The function performs with time complexity of O(n) where n is the number 
 * of data elements already in the heap.
 * 
 * @PARAM:
 * heap				pointer to heap data structure.
 * is_match			matching function pointer.
 * match_param		helpful parameter for is_match fucntion.
 * 
 * @RETURN:
 * HeapRemove() function will return the removed data to the user as void 
 * pointer. If a match was not found within the heap, NULL will be returned.
 * 
 * @WARNINGS:
 * *Do not place NULL as input of heap or is_match. Such a thing will lead to 
 *  undefined behavior.
 * *is_match matching function is expected to return 1 if data is matched to 
 *  match_param, and 0 otherwise. Any other values returned by this function 
 *  will result in undefined behavior (see heap_match_func_ty matching function 
 *  signature specifications for further details).
 */
void *HeapRemove(heap_ty *heap, heap_match_func_ty is_match, void *match_param);

#endif /* __HEAP_H__ */
