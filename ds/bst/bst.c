/******************************************************************************
 											
 		filename:			bst.c			
 		written by:     	Orit			
 		reviewed by:    	Leah
 		last edit:      	09.09.2020		
 											
 ******************************************************************************/
#include <assert.h>		/* assert()								*/
#include <string.h>		/* memset()								*/
#include <stdlib.h>		/* malloc(), free()						*/

#ifndef NDEBUG
#include <stdio.h>    /* fprintf() */
#define ALLOCATION_FAILED {\
                            fprintf (stderr, "malloc failed"\
                             " at %s, line %d.\n", __FILE__, __LINE__);\
                          }
#endif /* NDEBUG */

#include "bst.h"

enum BST_CHILDREN {LEFT, RIGHT, NUMBER_OF_CHILDREN};
enum IS_FOUND {FOUND, NOT_FOUND};
enum FOR_EACH_IS_SUCCESS {FOR_EACH_SUCCESS, FOR_EACH_FAIL};

struct node
{
	node_ty *parent;
	node_ty *children[NUMBER_OF_CHILDREN];
	const void *data;
};

struct bst
{
	bst_cmp_func_ty compare;
	void *param;
	node_ty root_stub;
};

/******************************************************************************/
/*Static functions declarations:*/
static node_ty *ExtractNodeFromIterIMP(iter_ty iter);
static iter_ty InsertNodeToIterIMP(node_ty *node);
static iter_ty GoToChildIMP(iter_ty iter, int child_num);
static iter_ty GoToParentIMP(iter_ty iter);
static void ConnectParentWithChildIMP(node_ty *parent, node_ty *child, 
									int child_num);
static iter_ty LoopToLeftIMP(iter_ty iter);
static iter_ty LoopToRightIMP(iter_ty iter);
static iter_ty LoopToWantedParentIMP(iter_ty current, int child_num);
static iter_ty SwitchChildIMP(node_ty *old_child, node_ty *new_child);
static int NumOfChildrenIMP(node_ty *node);
static void FreeNodeIMP(iter_ty iter);
static void SetNewNodeAsLeafWithDataIMP(node_ty *new_node, const void *data);

/******************************************************************************/
bst_ty *BSTCreate(bst_cmp_func_ty compare, void *param)
{
	bst_ty *new_bst = NULL;
	node_ty *root_stub = NULL;
	
	assert(NULL != compare);
	
	new_bst = (bst_ty *)malloc(sizeof(*new_bst));
	if(NULL == new_bst)
	{
#ifndef NDEBUG
		ALLOCATION_FAILED;
#endif
		return NULL;
	}
	memset(new_bst, 0, sizeof(*new_bst));
	new_bst->compare = compare;
	new_bst->param = param;
	root_stub = (node_ty *)&new_bst->root_stub;
	root_stub->parent = NULL;
	SetNewNodeAsLeafWithDataIMP(root_stub, NULL);
	
	return new_bst;
}

void BSTDestroy(bst_ty *bst)
{
	iter_ty iter = NULL;
	iter_ty parent = NULL;
	
	assert(NULL != bst);
	
	iter = BSTBegin(bst);
	while(1 != BSTIsEmpty(bst))
	{
		while(NULL != GoToChildIMP(iter, RIGHT))
		{
			iter = BSTNext(iter);
		}
		parent = SwitchChildIMP(ExtractNodeFromIterIMP(iter), NULL);
		FreeNodeIMP(iter);
		iter = parent;
	}
	memset(bst, 0, sizeof(bst_ty));
	free(bst);
}

size_t BSTSize(const bst_ty *bst)
{
	iter_ty iter = NULL;
	iter_ty end = NULL;
	size_t counter = 0;
	
	assert(NULL != bst);
	
	iter = BSTBegin((bst_ty *)bst);
	end = BSTEnd((bst_ty *)bst);
	while(end != iter)
	{
		++counter;
		iter = BSTNext(iter);
	}
	
	return counter;
}

int BSTIsEmpty(const bst_ty *bst)
{
	assert(NULL != bst);
	
	return (NULL == bst->root_stub.children[LEFT]);
}

iter_ty BSTInsert(bst_ty *bst, const void *data)
{
	iter_ty iter = NULL;
	node_ty *inserted_node = NULL;
	int next_child = LEFT;
	
	assert(NULL != bst);
	assert(NULL != data);
	
	inserted_node = (node_ty *)malloc(sizeof(*inserted_node));
	if(NULL == inserted_node)
	{
#ifndef NDEBUG
		ALLOCATION_FAILED;
#endif
		return NULL;
	}
	memset(inserted_node, 0, sizeof(*inserted_node));
	SetNewNodeAsLeafWithDataIMP(inserted_node, data);
	iter = InsertNodeToIterIMP(&bst->root_stub);
	while(NULL != GoToChildIMP(iter, next_child))
	{
		iter = GoToChildIMP(iter, next_child);
		next_child = (0 < bst->compare(data, (const void *)BSTGetData(iter),
							bst->param));
	}
	ConnectParentWithChildIMP(ExtractNodeFromIterIMP(iter), inserted_node, 
								next_child);
	
	return inserted_node;
}

void BSTRemove(iter_ty iter)
{
	node_ty *node_to_remove = NULL;
	node_ty *next = NULL;
	
	assert(NULL != iter);
	assert(NULL != GoToParentIMP(iter));
	
	node_to_remove = ExtractNodeFromIterIMP(iter);
	if(NUMBER_OF_CHILDREN == NumOfChildrenIMP(node_to_remove))
	{
		next = ExtractNodeFromIterIMP(BSTNext(iter));
		SwitchChildIMP(next, next->children[RIGHT]);
		SwitchChildIMP(node_to_remove, next);
		ConnectParentWithChildIMP(next, node_to_remove->children[LEFT], LEFT);
		ConnectParentWithChildIMP(next, node_to_remove->children[RIGHT], RIGHT);
	}
	else
	{
		next = (NULL != node_to_remove->children[LEFT]) ? 
						node_to_remove->children[LEFT] : 
						node_to_remove->children[RIGHT];
		SwitchChildIMP(node_to_remove, next);
	}
	FreeNodeIMP(node_to_remove);
}

iter_ty BSTBegin(bst_ty *bst)
{
	iter_ty iter = NULL;
	
	assert(NULL != bst);
	
	iter = InsertNodeToIterIMP(&bst->root_stub);
	iter = LoopToLeftIMP(iter);
	
	return iter;
}

iter_ty BSTEnd(bst_ty *bst)
{
	assert(NULL != bst);
	
	return InsertNodeToIterIMP(&bst->root_stub);
}

iter_ty BSTPrev(iter_ty current)
{
	iter_ty prev = NULL;
	
	assert(NULL != current);
	
	if(NULL != GoToChildIMP(current, LEFT))
	{
		prev = GoToChildIMP(current, LEFT);
		prev = LoopToRightIMP(prev);
	}
	else
	{
		prev = LoopToWantedParentIMP(current, RIGHT);
	}
	
	return prev;
}

iter_ty BSTNext(iter_ty current)
{
	iter_ty next = NULL;
	
	assert(NULL != current);
	
	if(NULL != GoToChildIMP(current, RIGHT))
	{
		next = GoToChildIMP(current, RIGHT);
		next = LoopToLeftIMP(next);
	}
	else
	{
		next = LoopToWantedParentIMP(current, LEFT);
	}
	
	return next;
}

int BSTIterIsEqual(iter_ty iter1, iter_ty iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}

void *BSTGetData(iter_ty iter)
{
	assert(NULL != iter);
	
	return (void *)(ExtractNodeFromIterIMP(iter))->data;
}

iter_ty BSTFind(bst_ty *bst, const void *data)
{
	iter_ty iter = NULL;
	int is_found = NOT_FOUND;
	int next_child = LEFT;
	
	assert(NULL != bst);
	assert(NULL != data);
	
	iter = InsertNodeToIterIMP(&bst->root_stub);
	while(FOUND != is_found && NULL != GoToChildIMP(iter, next_child))
	{
		iter = GoToChildIMP(iter, next_child);
		is_found = bst->compare(data, (const void *)BSTGetData(iter), 
									bst->param);
		next_child = (0 < is_found);
	}
	if(FOUND != is_found)
	{
		iter = InsertNodeToIterIMP(&bst->root_stub);
	}
	
	return iter;
}

int BSTForeach(iter_ty from, iter_ty to,
				bst_operation_func_ty operation_function, void *param)
{
	iter_ty iter = NULL;
	int is_success = FOR_EACH_SUCCESS;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != operation_function);
	
	for(iter = from; to != iter && FOR_EACH_SUCCESS == is_success; 
		iter =  BSTNext(iter))
	{
		is_success = operation_function(BSTGetData(iter), param);
	}
	
	return is_success;
}

/******************************************************************************/
/*Static functions:*/
static node_ty *ExtractNodeFromIterIMP(iter_ty iter)
{
	assert(NULL != iter);
	
	return iter;
}

static iter_ty InsertNodeToIterIMP(node_ty *node)
{
	assert(NULL != node);
	
	return node;
}

static iter_ty GoToChildIMP(iter_ty iter, int child_num)
{
	assert(NULL != iter);
	
	return iter->children[child_num];
}

static iter_ty GoToParentIMP(iter_ty iter)
{
	assert(NULL != iter);
	
	return iter->parent;
}

static void ConnectParentWithChildIMP(node_ty *parent, node_ty *child, 
									int child_num)
{
	assert(NULL != parent);
	assert(-1 < child_num && 2 > child_num);
	
	parent->children[child_num] = child;
	if(NULL != child)
	{
		child->parent = parent;
	}
}

static iter_ty LoopToLeftIMP(iter_ty iter)
{
	assert(NULL != iter);
	
	while(NULL != GoToChildIMP(iter, LEFT))
	{
		iter = GoToChildIMP(iter, LEFT);
	}
	
	return iter;
}

static iter_ty LoopToRightIMP(iter_ty iter)
{
	assert(NULL != iter);
	
	while(NULL != GoToChildIMP(iter, RIGHT))
	{
		iter = GoToChildIMP(iter, RIGHT);
	}
	
	return iter;
}

static iter_ty LoopToWantedParentIMP(iter_ty current, int child_num)
{
	iter_ty parent = NULL;
	iter_ty child = NULL;
	
	assert(NULL != current);
	
	child = current;
	parent = current;
	while(child != GoToChildIMP(parent, child_num) && 
			NULL != GoToParentIMP(parent))
	{
		child = parent;
		parent = GoToParentIMP(child);
	}
	
	return parent;
}

static iter_ty SwitchChildIMP(node_ty *old_child, node_ty *new_child)
{
	node_ty *parent = NULL;
	int child_num = LEFT;
	
	assert(NULL != old_child);

	
	parent = old_child->parent;
	child_num = (old_child == parent->children[RIGHT]);
	ConnectParentWithChildIMP(parent, new_child, child_num);
	
	return parent;
}

static int NumOfChildrenIMP(node_ty *node)
{
	int num_of_children = 0;
	
	assert(NULL != node);
	
	num_of_children += (NULL != node->children[LEFT]);
	num_of_children += (NULL != node->children[RIGHT]);
	
	return num_of_children;
}

static void FreeNodeIMP(iter_ty iter)
{
	node_ty *node = NULL;
	
	assert(NULL != iter);
	
	node = ExtractNodeFromIterIMP(iter);
	
	memset(node, 0, sizeof(node_ty));
	free(node);
}

static void SetNewNodeAsLeafWithDataIMP(node_ty *new_node, const void *data)
{
	assert(NULL != new_node);
	
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	new_node->data = data;
}
