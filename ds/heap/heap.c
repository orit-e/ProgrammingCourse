/******************************************************************************
 											
				File name:			heap.c			
				Written by:     	Orit			
				Reviewed by:    	Yona
 											
 ******************************************************************************/
#include <assert.h>		/* assert()			    */
#include <string.h>		/* memset()				*/
#include <stdlib.h>		/* malloc(), free()		*/

#include "heap.h"
#include "vector.h"

/******************************************************************************/
/*					struct and typedefs declarations:				  */
typedef void (*push_status_func_ty)(heap_ty *heap, size_t last_index);
typedef void (*pop_func_ty)(heap_ty *heap, size_t parent, size_t last_index);

struct heap
{
    vector_t *vector;
    heap_cmp_func_ty compare;
    void *param;
};

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
static void EnvVectorPopIMP(heap_ty *heap, size_t last_index);
static void SwapDataIMP(heap_ty *heap, size_t index1, size_t index2);
static void HeapifyUpIMP(heap_ty *heap, size_t new_data_index);
static void HeapifyDownIMP(heap_ty *heap, size_t parent, size_t last_index);
static void RemoveAndReorderIMP(heap_ty *heap, size_t to_pop, 
                                size_t last_index);
static size_t GetChildWithHigherPriority(heap_ty *heap, size_t parent, 
                                            size_t last_index);
static void DoNothingIMP(heap_ty *heap, size_t parent, size_t last_index);
static size_t GetParentIMP(size_t child);
static size_t GetLeftChildIMP(size_t last_index, size_t parent);
static size_t GetRightChildIMP(size_t last_index, size_t parent);

/******************************************************************************/
/*								Heap functions:								  */
heap_ty *HeapCreate(heap_cmp_func_ty compare, void *param)
{
    heap_ty *new_heap = NULL;

    assert(NULL != compare);

    new_heap = (heap_ty *)malloc(sizeof(heap_ty));
    if(NULL == new_heap)
    {
        return NULL;
    }
    memset(new_heap, 0, sizeof(heap_ty));
    new_heap->vector = VectorCreate(8);
    if(NULL == new_heap->vector)
    {
        free(new_heap);
        return NULL;
    }
    new_heap->compare = compare;
    new_heap->param = param;
    
    return new_heap;
}

void HeapDestroy(heap_ty *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    VectorDestroy(heap->vector);
    memset(heap, 0, sizeof(heap_ty));
    free(heap);
}

int HeapPush(heap_ty *heap, void *data)
{
    push_status_func_ty status_func = NULL;
    static push_status_func_ty arr_push[2] = {HeapifyUpIMP, EnvVectorPopIMP};
    static int status = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);
    assert(NULL != data);

    status = VectorPush(heap->vector, data);
    status_func = arr_push[status];
    status_func(heap, HeapSize(heap) - 1);

    return status;
}

void HeapPop(heap_ty *heap)
{
    size_t last_index = 0;
    
    assert(NULL != heap);
    assert(NULL != heap->vector);

    last_index = HeapSize(heap) - 1;
    RemoveAndReorderIMP(heap, 0, last_index);
}

void *HeapPeek(const heap_ty *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    return VectorGetElement(heap->vector, 0);
}

size_t HeapSize(const heap_ty *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    return VectorSize(heap->vector);
}

int HeapIsEmpty(const heap_ty *heap)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);

    return (0 == VectorSize(heap->vector));
}

void *HeapRemove(heap_ty *heap, heap_match_func_ty is_match, void *match_param)
{
    void *data = NULL;
    size_t i = 0;
    size_t heap_size = 0;
    int is_matched = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);
    assert(NULL != is_match);

    heap_size = VectorSize(heap->vector);
    for(i = 0; heap_size > i && 1 != is_matched; ++i)
    {
        is_matched = is_match(VectorGetElement(heap->vector, i), match_param);
    }
    if(1 == is_matched)
    {
        data = VectorGetElement(heap->vector, i - 1);
        RemoveAndReorderIMP(heap, i - 1, heap_size - 1);
    }

    return data;
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
static void EnvVectorPopIMP(heap_ty *heap, size_t last_index)
{
    assert(NULL != heap);
    assert(NULL != heap->vector);
    
    VectorPop(heap->vector);

    (void)last_index;
}

static void SwapDataIMP(heap_ty *heap, size_t index1, size_t index2)
{
    void *temp = NULL;

    assert(NULL != heap);
    assert(NULL != heap->vector);
    assert(HeapSize(heap) > index1);
    assert(HeapSize(heap) > index2);

    temp = VectorGetElement(heap->vector, index1);
    VectorSetElement(heap->vector, index1, 
                        VectorGetElement(heap->vector, index2));
    VectorSetElement(heap->vector, index2, temp);
}

static void HeapifyUpIMP(heap_ty *heap, size_t new_data_index)
{
    size_t parent = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);

    if(0 == new_data_index)
    {
        return;
    }
    parent = GetParentIMP(new_data_index);
    if(0 < heap->compare(VectorGetElement(heap->vector, new_data_index), 
                            VectorGetElement(heap->vector, parent), 
                            heap->param))
    {
        SwapDataIMP(heap, parent, new_data_index);
        HeapifyUpIMP(heap, parent);
    }
}

static void HeapifyDownIMP(heap_ty *heap, size_t parent, size_t last_index)
{
    size_t compared_child = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);

    compared_child = GetChildWithHigherPriority(heap, parent, last_index);
    
    if(parent < compared_child && 
        0 > heap->compare(VectorGetElement(heap->vector, parent), 
                            VectorGetElement(heap->vector, compared_child), 
                            heap->param))
    {
        SwapDataIMP(heap, parent, compared_child);
        HeapifyDownIMP(heap, compared_child, last_index);
    }
}

static size_t GetChildWithHigherPriority(heap_ty *heap, size_t parent, 
                                            size_t last_index)
{
    size_t left_child = 0;
    size_t right_child = 0;

    assert(NULL != heap);
    assert(NULL != heap->vector);

    left_child = GetLeftChildIMP(last_index, parent);
    right_child = GetRightChildIMP(last_index, parent);

    if(0 == right_child)
    {
        return left_child;
    }
    else
    {
        return ((0 <= heap->compare(VectorGetElement(heap->vector, left_child), 
                                VectorGetElement(heap->vector, right_child), 
                                    heap->param)) ? left_child : right_child);
    }
}

static void DoNothingIMP(heap_ty *heap, size_t parent, size_t last_index)
{
    (void)heap;
    (void)parent;
    (void)last_index;
}

static void RemoveAndReorderIMP(heap_ty *heap, size_t to_pop, 
                                size_t last_index)
{
    pop_func_ty is_empty_func = NULL;
    static pop_func_ty arr_is_empty[2] = {HeapifyDownIMP, 
                                            DoNothingIMP};
    
    assert(NULL != heap);
    assert(NULL != heap->vector);

    SwapDataIMP(heap, to_pop, last_index);
    VectorPop(heap->vector);
    is_empty_func = arr_is_empty[(0 == last_index)];
    is_empty_func(heap, to_pop, last_index - 1);
    HeapifyUpIMP(heap, to_pop);
}

static size_t GetParentIMP(size_t child)
{
    return ((child - 1) / 2);
}

static size_t GetLeftChildIMP(size_t last_index, size_t parent)
{
    return ((last_index >= parent * 2 + 1) ? parent * 2 + 1 : 0);
}

static size_t GetRightChildIMP(size_t last_index, size_t parent)
{
    return ((last_index >= parent * 2 + 2) ? parent * 2 + 2 : 0);
}
