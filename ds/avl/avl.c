/******************************************************************************
 											
				filename:			avl.c			
				written by:     	Orit			
				reviewed by:    	Adi
				last edit:      	23.09.2020		
 											
 ******************************************************************************/
#include <assert.h>		/*	assert()			*/
#include <string.h>		/*	memset()			*/
#include <stdlib.h>		/*	malloc(), free()	*/

#include "avl.h"

/******************************************************************************/
/*					enum and structs typedefs and declarations:				  */
typedef enum AVL_CHILDREN 
{
	LEFT,
	RIGHT,
	NUMBER_OF_CHILDREN
}avl_children_ty;

typedef struct node node_ty;
struct node
{
	node_ty *children[NUMBER_OF_CHILDREN];
	void *data;
	size_t height;
};

struct avl
{
	avl_cmp_func_ty cmp_func;
	void *param;
	node_ty *root;
};

/******************************************************************************/
/*				Singly recursive static functions declarations:				  */
static void RecursiveDestroyIMP(node_ty *node);
static size_t RecursiveSizeCountIMP(node_ty *node);
static node_ty *RecursiveInsertIMP(avl_ty *avl, node_ty *root, 
									node_ty *inserted_node);
static void *RecursiveFindIMP(const avl_ty *avl, node_ty *node, 
								const void *to_find);

/******************************************************************************/
/*				All of the static functions for AVLRemove only:				  */
static node_ty *RecursiveRemoveIMP(avl_ty *avl, node_ty *root, 
									const void *data);
static node_ty *DeleteNodeIMP(node_ty *node);
static node_ty *DeleteNodeWithNoChildIMP(node_ty *node);
static node_ty *DeleteNodeWithOneChildIMP(node_ty *node);
static node_ty *DeleteNodeWithTwoChildrenIMP(node_ty *node);
static node_ty *FindSubRootReplacementIMP(node_ty *sub_root, 
											avl_children_ty next_child);
static void FreeNodeIMP(node_ty *node);

/******************************************************************************/
/*					Static functions for tree balancing:					  */
static int RootBalanceIMP(node_ty *root);
static int IsBalancedIMP(node_ty *root);
static int GetHeavySide(node_ty *root);
static node_ty *BalanceTreeIMP(node_ty *root);
static node_ty *SingleRotIMP(node_ty *root, avl_children_ty rot_side);

/******************************************************************************/
/*				Recursive static functions for AVLForeach:					  */
static int RecursiveForEachPreIMP(node_ty *node, avl_operation_func_ty op_func, 
                            		void *param);
static int RecursiveForEachInIMP(node_ty *node, avl_operation_func_ty op_func, 
                                	void *param);
static int RecursiveForEachPostIMP(node_ty *node, 
									avl_operation_func_ty op_func, 
                                	void *param);

/******************************************************************************/
/*						Some helpful static functions:						  */
static int NumOfChildrenIMP(node_ty *node);
static node_ty *CreateNewNodeIMP(const void *data);
static size_t CalculateNodeHeightIMP(node_ty *node);

/******************************************************************************/
/*								AVL functions:								  */
avl_ty *AVLCreate(avl_cmp_func_ty cmp_func, void *param)
{
	avl_ty *new_avl = NULL;
    
	assert(NULL != cmp_func);
	
	new_avl = (avl_ty *)malloc(sizeof(*new_avl));
	if(NULL == new_avl)
	{
		return NULL;
	}
	
	memset(new_avl, 0, sizeof(*new_avl));
	new_avl->cmp_func = cmp_func;
	new_avl->param = param;
	new_avl->root = NULL;
	
	return new_avl;
}

void AVLDestroy(avl_ty *avl) /* Post Order */
{
	assert(NULL != avl);

	RecursiveDestroyIMP(avl->root);
	memset(avl, 0, sizeof(avl_ty));
	free(avl);
}

size_t AVLSize(const avl_ty *avl) /* Pre Order */
{
	assert(NULL != avl);

	return RecursiveSizeCountIMP(avl->root);
}

int AVLIsEmpty(const avl_ty *avl)
{
    assert(NULL != avl);
	
	return (NULL == avl->root);
}

int AVLInsert(avl_ty *avl, void *data)
{
	node_ty *inserted_node = NULL;

	assert(NULL != avl);
	assert(NULL != data);

	inserted_node = CreateNewNodeIMP(data);
	if(NULL == inserted_node)
	{
		return 1;
	}
	avl->root = RecursiveInsertIMP(avl, avl->root, inserted_node);
        
	return 0;
}

void AVLRemove(avl_ty *avl, const void *data)
{
	assert(NULL != avl);
	assert(NULL != data);

	avl->root = RecursiveRemoveIMP(avl, avl->root, data);
}

size_t AVLHeight(const avl_ty *avl)
{
	assert(NULL != avl);

	return CalculateNodeHeightIMP(avl->root);
}

void *AVLFind(const avl_ty *avl, const void *to_find)
{
	assert(NULL != avl);
	assert(NULL != to_find);

	return RecursiveFindIMP(avl, avl->root, to_find);
}

int AVLForeach(avl_ty *avl, avl_operation_func_ty op_func, 
			   void *param, traverse_ty traverse)
{
	assert(NULL != avl);
	assert(NULL != op_func);
	assert(NULL != param);
	assert(PREORDER <= traverse && POSTORDER >= traverse);

	if(PREORDER == traverse)
	{
		return RecursiveForEachPreIMP(avl->root, op_func, param);
	}
	if(INORDER == traverse)
	{
		return RecursiveForEachInIMP(avl->root, op_func, param);
	}

	return RecursiveForEachPostIMP(avl->root, op_func, param);
}

/******************************************************************************/
/*				Singly recursive static functions definitions:				  */
static void RecursiveDestroyIMP(node_ty *node)
{
	if(NULL == node)
	{
		return;
	}
	RecursiveDestroyIMP(node->children[LEFT]);
	RecursiveDestroyIMP(node->children[RIGHT]);
	FreeNodeIMP(node);
}

static node_ty *RecursiveInsertIMP(avl_ty *avl, node_ty *root, 
									node_ty *inserted_node)
{
	avl_children_ty next_child = 0;
	assert(NULL != avl);
	assert(NULL != inserted_node);

	if(NULL == root)
	{
        return inserted_node;
	}
	next_child = (0 < avl->cmp_func(inserted_node->data, root->data, 
									avl->param));
	root->children[next_child] = RecursiveInsertIMP(avl, 
										root->children[next_child], 
										inserted_node);
	root->height = CalculateNodeHeightIMP(root);
	if(1 != IsBalancedIMP(root))
	{
		root = BalanceTreeIMP(root);
	}

	return root;
}

static size_t RecursiveSizeCountIMP(node_ty *node)
{
	if(NULL == node)
	{
		return 0;
	}

	return (RecursiveSizeCountIMP(node->children[LEFT]) + 
            RecursiveSizeCountIMP(node->children[RIGHT]) + 1);
}

static void *RecursiveFindIMP(const avl_ty *avl, node_ty *node, 
								const void *to_find)
{
	int cmp_result = 1;

	assert(NULL != avl);
	assert(NULL != to_find);

	if(NULL == node)
	{
		return NULL;
	}
	cmp_result = avl->cmp_func(to_find, node->data, avl->param);
	if(0 == cmp_result)
	{
		return node->data;
	}

	return RecursiveFindIMP(avl, node->children[(0 < cmp_result)], to_find);
}

/******************************************************************************/
/*				All of the static functions for AVLRemove only:				  */
static node_ty *RecursiveRemoveIMP(avl_ty *avl, node_ty *root, const void *data)
{
	int cmp_result = 1;

	assert(NULL != avl);
	assert(NULL != data);

	if(NULL == root)
	{
		return NULL;
	}
	cmp_result = avl->cmp_func(data, root->data, avl->param);
	if(0 == cmp_result)
	{
        return DeleteNodeIMP(root);
	}        
	root->children[(0 < cmp_result)] = RecursiveRemoveIMP(avl, 
											root->children[(0 < cmp_result)], 
											data);
	root->height = CalculateNodeHeightIMP(root);
	if(1 != IsBalancedIMP(root))
	{
		root = BalanceTreeIMP(root);
	}

	return root;
}

static node_ty *DeleteNodeIMP(node_ty *node)
{
	int num_of_children = 0;

	assert(NULL != node);
	
	num_of_children = NumOfChildrenIMP(node);
	if(1 == num_of_children)
	{
		return DeleteNodeWithOneChildIMP(node);
	}
	else if(2 == num_of_children)
	{
		return DeleteNodeWithTwoChildrenIMP(node);
	}
	else
	{
		return DeleteNodeWithNoChildIMP(node);
	}
}

static node_ty *DeleteNodeWithNoChildIMP(node_ty *node)
{
	assert(NULL != node);

	FreeNodeIMP(node);

	return NULL;
}

static node_ty *DeleteNodeWithOneChildIMP(node_ty *node)
{
	node_ty *child = NULL;
	
	assert(NULL != node);

	child = (node_ty *)((unsigned long)node->children[LEFT] |
						(unsigned long)node->children[RIGHT]);
	FreeNodeIMP(node);
	
	return child;
}

static node_ty *DeleteNodeWithTwoChildrenIMP(node_ty *node)
{
	node_ty *replacement_node = NULL;

	assert(NULL != node);

	replacement_node = FindSubRootReplacementIMP(node, RIGHT);
	replacement_node->children[LEFT] = node->children[LEFT];
	replacement_node->children[RIGHT] = node->children[RIGHT];
	replacement_node->height = CalculateNodeHeightIMP(replacement_node);
	FreeNodeIMP(node);
	if(1 != IsBalancedIMP(replacement_node))
	{
		replacement_node = BalanceTreeIMP(replacement_node);
	}

	return replacement_node;
}

static node_ty *FindSubRootReplacementIMP(node_ty *sub_root, 
											avl_children_ty next_child)
{
	node_ty *sub_root_replacement = NULL;

	assert(NULL != sub_root);
	assert(LEFT <= next_child && NUMBER_OF_CHILDREN > next_child);

	if(NULL == sub_root->children[next_child]->children[LEFT])
	{
		sub_root_replacement = sub_root->children[next_child];
        sub_root->children[next_child] = sub_root_replacement->children[RIGHT];
	}   
	else
	{
		sub_root_replacement = 
                FindSubRootReplacementIMP(sub_root->children[next_child], LEFT);
	}
	sub_root->height = CalculateNodeHeightIMP(sub_root);

	return sub_root_replacement;
}

/******************************************************************************/
/*					Static functions for tree balancing:					  */
static int RootBalanceIMP(node_ty *root)
{
	int num_of_children = 0;
	int balance = 0;

	assert(NULL != root);

	num_of_children = NumOfChildrenIMP(root);
	if(2 == num_of_children)
	{
		balance = root->children[RIGHT]->height - root->children[LEFT]->height;
	}
	else if(1 == num_of_children && NULL != root->children[RIGHT])
	{
		balance = root->children[RIGHT]->height + 1;
	}
	else if(1 == num_of_children && NULL != root->children[LEFT])
	{
		balance = -root->children[LEFT]->height - 1;
	}

	return balance;
}

static int IsBalancedIMP(node_ty *root)
{
	int balance = 0;

	assert(NULL != root);

	balance = RootBalanceIMP(root);

	return (-2 < balance && 2 > balance);
}

static int GetHeavySide(node_ty *root)
{
	return (0 > RootBalanceIMP(root) ? LEFT : RIGHT);
}

static node_ty *BalanceTreeIMP(node_ty *root)
{
	int root_heavy_side = 0;
	int child_heavy_side = 0;

	assert(NULL != root);

	root_heavy_side = GetHeavySide(root);
	child_heavy_side = GetHeavySide(root->children[root_heavy_side]);
	if(root_heavy_side != child_heavy_side)
	{
		root->children[root_heavy_side] = 
							SingleRotIMP(root->children[root_heavy_side], 
											child_heavy_side);
	}

	return SingleRotIMP(root, root_heavy_side);
}

static node_ty *SingleRotIMP(node_ty *root, avl_children_ty rot_side)
{
	node_ty *rotating_child = NULL;

	assert(NULL != root);
	assert(LEFT <= rot_side && NUMBER_OF_CHILDREN > rot_side);

	rotating_child = root->children[rot_side];
	root->children[rot_side] = rotating_child->children[rot_side ^ RIGHT];
	rotating_child->children[rot_side ^ RIGHT] = root;
	root->height = CalculateNodeHeightIMP(root);
	rotating_child->height = CalculateNodeHeightIMP(rotating_child);
	
	return rotating_child;
}

/******************************************************************************/
/*				Recursive static functions for AVLForeach:					  */
static int RecursiveForEachPreIMP(node_ty *node, avl_operation_func_ty op_func, 
                                	void *param)
{
	int status = 0;

	assert(NULL != op_func);
	assert(NULL != param);

	if(NULL == node)
	{
		return status;
	}
	status = op_func(node->data, param);
	if(0 != status)
	{
		return status;
	}
	status = RecursiveForEachPreIMP(node->children[LEFT], op_func, param);
	if(0 != status)
	{
		return status;
	}
	status = RecursiveForEachPreIMP(node->children[RIGHT], op_func, param);

	return status;
}

static int RecursiveForEachInIMP(node_ty *node, avl_operation_func_ty op_func, 
        							void *param)
{
	int status = 0;

	assert(NULL != op_func);
	assert(NULL != param);

	if(NULL == node)
	{
		return status;
	}
	status = RecursiveForEachInIMP(node->children[LEFT], op_func, param);
	if(0 != status)
	{
		return status;
	}
	status = op_func(node->data, param);
	if(0 != status)
	{
		return status;
	}
	status = RecursiveForEachInIMP(node->children[RIGHT], op_func, param);

	return status;
}

static int RecursiveForEachPostIMP(node_ty *node, 
									avl_operation_func_ty op_func, 
                                	void *param)
{
	int status = 0;

	assert(NULL != op_func);
	assert(NULL != param);

	if(NULL == node)
	{
		return status;
	}
	status = RecursiveForEachPostIMP(node->children[LEFT], op_func, param);
	if(0 != status)
	{
		return status;
	}
	status = RecursiveForEachPostIMP(node->children[RIGHT], op_func, param);
	if(0 != status)
	{
		return status;
	}
	status = op_func(node->data, param);

	return status;
}

/******************************************************************************/
/*						Some helpful static functions:						  */
static node_ty *CreateNewNodeIMP(const void *data)
{
	node_ty *new_node = (node_ty *)malloc(sizeof(node_ty));
	
	if(NULL == new_node)
	{
		return NULL;
	}
	memset(new_node, 0, sizeof(node_ty));
	new_node->data = (void *)data;

	return new_node;
}

static size_t CalculateNodeHeightIMP(node_ty *node)
{
	size_t height = 0;
	int num_of_children = 0;

	assert(NULL != node);

	num_of_children = NumOfChildrenIMP(node);
	if(1 == num_of_children)
	{
		height = (NULL == node->children[LEFT]) ? 
                    node->children[RIGHT]->height + 1 :
                    node->children[LEFT]->height + 1;
	}
	else if(2 == num_of_children)
	{
		height = (node->children[RIGHT]->height > 
                    node->children[LEFT]->height) ? 
                    node->children[RIGHT]->height + 1 :
                    node->children[LEFT]->height + 1;
	}

	return height;
}

static void FreeNodeIMP(node_ty *node)
{
	assert(NULL != node);

	memset(node, 0, sizeof(node_ty));
	free(node);
}

static int NumOfChildrenIMP(node_ty *node)
{
	assert(NULL != node);

	return ((NULL != node->children[LEFT]) + (NULL != node->children[RIGHT]));
}