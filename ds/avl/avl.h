/******************************************************************************
 											
				filename:			avl.h			
				written by:     	Orit			
				reviewed by:    	Adi
				last edit:      	23.09.2020		
 											
 ******************************************************************************/
#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h>	/* size_t */

typedef enum TRAVERSE
{
    PREORDER,
    INORDER,
    POSTORDER
}traverse_ty;

typedef struct avl avl_ty;

/** 
 * @DESCRIPTION:
 * avl_cmp_func_ty comparison function pointer is given by the user at 
 * AVLCreate() and is used by avl functions to sort and find inserted data. The 
 * comparison function will receive and compare data1 and data2 with the 
 * optional help of param, also given by user at AVLCreate(), and will determin
 * their order.
 * avl_cmp_func_ty comparison function pointer will return an integer 
 * indicating the result of the comparison as follows:
 * *0 if the data1 and data2 comparison keys are equal.
 * *a negative value if data1 comparison key is less than data2 comparison key.
 * *a positive value if data1 comparison key is greater than data2 comparison 
 *  key.
 *
 * @RETURN:
 * avl_cmp_func_ty comparison function pointer will return an integer less than,
 * equal to, or greater than zero if data1 comparison key is found, 
 * respectively, to be less than, to match, or be greater than data2 comparison 
 * key.
 */
typedef int(*avl_cmp_func_ty)(const void *data1, const void *data2, 
								void *param);

/**
 * @DESCRIPTION:
 * avl_operation_func_ty operation function pointer is given by the user at 
 * AVLForeach() and is used by the user to perform some operation on data placed
 * within some range of avl nodes refered to by the appropriate iterators (see 
 * AVLForeach() for further information).
 * The operation performed must not change the values of any of the comparison 
 * keys.
 *
 * @RETURN:
 * avl_operation_func_ty operation function pointer will return 0 upon success.
 * If an error occure whithin the operation function, and the user wishes to 
 * terminate the AVLForeach() function before its inner loop is finnished, the 
 * operation function must return any integer other than 0 (see AVLForeach() for
 * further information).
 *
 * @WARNINGS:
 * *It is required that op_func return value will be 0 in case of 
 *  success. Any other return value will be interpeted by AVLForeach() function 
 *  as a sign that op_func failed and will result in the terminating 
 *  of the function durring its run.
 * *op_func must not change the value of the comparison key placed 
 *  inside each data. Such a thing will lead to undefind behavior.
 */
typedef int(*avl_operation_func_ty)(void *data, void *param);

/**
 * @DESCRIPTION:
 * AVLCreate() function create new avl tree. The function receives cmp_func 
 * function pointer and param from user for with which the tree will be ordered 
 * (see avl_cmp_func_ty for compare function signiture specifications and also 
 * see AVLInsert(), AVLRemove() and AVLFind() for further information).
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * cmp_func		pointer to comparison function.
 * param		helping parameter for cmp_func.
 *
 * @RETURN:
 * AVLCreate() function return a pointer to a new and empty avl tree.
 * In case of malloc failure, NULL will be returned.
 *
 * @ERROR:
 * If malloc fails, the function will return NULL.
 * 
 * @WARNING:
 * Do not place NULL as input of cmp_func. Such a thing will lead to undefind 
 * behavior.
 */
avl_ty *AVLCreate(avl_cmp_func_ty cmp_func, void *param);

/**
 * @DESCRIPTION:
 * AVLDestroy() function destroys given avl data structure with recursion.
 * The function performs with time complexity of O(n) where n is the number of 
 * nodes placed in the tree.
 * 
 * @PARAM:
 * avl			pointer to avl tree.
 *
 * @WARNING:
 * Do not place NULL as input of avl. Such a thing will lead to undefind 
 * behavior.
 */
void AVLDestroy(avl_ty *avl); /* Post Order */

/**
 * @DESCRIPTION:
 * AVLSize() function counts with recursion and return the number of nodes in 
 * given avl tree.
 * The function performs with time complexity of O(n) where n is the number of 
 * nodes placed in the tree.
 * 
 * @PARAM:
 * avl			pointer to avl tree.
 *
 * @RETURN:
 * AVLSize() function return the number of nodes in given avl tree as a size_t 
 * integer.
 *
 * @WARNING:
 * Do not place NULL as input of avl. Such a thing will lead to undefind 
 * behavior.
 */
size_t AVLSize(const avl_ty *avl); /* Pre Order */

/**
 * @DESCRIPTION:
 * AVLIsEmpty() function checks whether given avl tree is empty or not.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * avl			pointer to avl tree
 *
 * @RETURN:
 * AVLIsEmpty() function will return boolean TRUE if avl is empty, otherwise 
 * boolean FALSE.
 *
 * @WARNING:
 * Do not place NULL as input of avl. Such a thing will lead to undefind 
 * behavior.
 */
int AVLIsEmpty(const avl_ty *avl);

/**
 * @DESCRIPTION:
 * AVLInsert() function creates a new node and place data in it. Than, with the 
 * help of  cmp_func given in AVLCreate() and with recursion, select the right 
 * place in the tree for the new node and connects it to avl tree. The function 
 * will than check if the tree is balanced and, if not, will continue to 
 * balance it.
 * The function performs with time complexity of O(logn) where n is the number 
 * of nodes placed in the tree.
 * 
 * @PARAM:
 * avl			pointer to avl tree.
 * data			data to be inserted to avl tree.
 *
 * @RETURN:
 * AVLInsert() function return 0 for success.
 * If malloc fails, 1 is returned.
 *
 * @ERROR:
 * If malloc fails, the function will return 1.
 *
 * @WARNINGS:
 * *Do not place NULL as input of avl or of data. Such a thing will lead to 
 *  undefind behavior.
 * *This avl tree relay on the fact that each key of data to be inserted is 
 *  unique and does not equal any other key already inserted to the tree. The 
 *  behavior of two eaul keys of two different inserted data instanses is 
 *  undefind.
 */
int AVLInsert(avl_ty *avl, void *data);

/**
 * @DESCRIPTION:
 * AVLRemove() function removes the node whose data is equal to data param, as 
 * matched by cmp_func, from the avl tree. The function will than check if the 
 * tree is balanced and, if not, will continue to balance it. The function uses 
 * recursion.
 * The function performs with time complexity of O(logn) where n is the number 
 * of nodes placed in the tree.
 * 
 * @PARAM:
 * avl			pointer to avl tree.
 * data			data to be removed from avl tree.
 *
 * @WARNINGS:
 * *Do not place NULL as input of avl or of data. Such a thing will lead to 
 *  undefind behavior.
 * *Trying to remove data the is not currently contained in avl tree will lead 
 *  to undefind behavior.
 */
void AVLRemove(avl_ty *avl, const void *data);

/**
 * @DESCRIPTION:
 * AVLHeight() function return the height of the given avl tree.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * avl			pointer to avl tree.
 *
 * @RETURN:
 * AVLHeight() function return the height of the given avl tree.
 *
 * @WARNINGS:
 * *Do not place NULL as input of avl. Such a thing will lead to undefind 
 *  behavior.
 * *Do not place an empty avl tree as input of avl. Such a thing will lead to 
 *  undefind behavior.
 */
size_t AVLHeight(const avl_ty *avl);

/**
 * @DESCRIPTION:
 * AVLFind() function receives avl and data to search for in avl tree and
 * return a pointer said data if found. The function uses recursion.
 * The function performs with time complexity of O(logn) where n is the number 
 * of nodes placed in the tree.
 * 
 * @PARAM:
 * avl			pointer to avl tree
 * data			data to search in avl tree
 *
 * @RETURN:
 * AVLFind() function return a pointer said data if found.
 * If data is not found, NULL will be returned.
 * 
 * @WARNINGS:
 * Do not place NULL as input of avl or data. Such a thing will lead to 
 * undefind behavior.
 */
void *AVLFind(const avl_ty *avl, const void *to_find);

/**
 * @DESCRIPTION:
 * AVLForeach() function receives avl tree and recursivly send the data stored 
 * in it to be operated by op_func with param (see typedef of 
 * avl_operation_func_ty for further details on the correct function 
 * signiture). If an error ocures in op_func durring the run, the fuction will 
 * stop its inner loop and exit with the returned value given by op_func (see 
 * WARNINGS for further detail). The order of tree traversal is chosen by the 
 * user with traverse (see enum TRAVERSE for further detail).
 * The function performs with complexity of O(n) where n is the number of nodes 
 * placed in the tree.
 * 
 * @param:
 * avl			Pointer to avl tree.
 * op_func		Pointer to operation function.
 * param		Helpful parameter for cmp_func.
 * traverse		Option from enum, PREORDER, INORDER, POSTORDER.
 *
 * @return:
 * AVLForeach() function return 0 if operation is successfull.
 * If an error ocures in op_func durring the run, the fuction will stop its 
 * inner recursive loop and exit with the returned value given by op_func.
 * 
 * @WARNINGS:
 * *Do not place NULL as input of op_func. Such a thing will lead to undefind 
 *  behavior.
 * *It is required that op_func return value will be 0 in case of success. Any 
 *  other return value will be interpeted by AVLForeach() function as a sign 
 *  that op_func failed and will result in the terminating of the function 
 *  durring its run.
 * *op_func must not change the value of the comparison key placed inside each 
 *  data. Such a thing will lead to undefind behavior.
 */
int AVLForeach(avl_ty *avl, avl_operation_func_ty op_func, 
			   void *param, traverse_ty traverse);


#endif /* __AVL_H__ */
