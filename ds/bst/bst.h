/******************************************************************************
 											
 		filename:			bst.h			
 		written by:     	Orit			
 		reviewed by:    	Leah
 		last edit:      	10.09.2020		
 											
 ******************************************************************************/
#ifndef __BST_H__
#define __BST_H__

#include <stddef.h> /* size_t */

typedef struct bst bst_ty;
typedef struct node node_ty;
typedef node_ty *iter_ty;

/* DESCRIPTION:
 * bst_cmp_func_ty comparison function pointer is given by the user at 
 * BSTCreate() and is used by bst functions to sort and find inserted data. The 
 * comparison function will receive and compare data1 and data2 with the 
 * optional help of param, also given by user at BSTCreate(), and will determin
 * their order.
 * bst_cmp_func_ty comparison function pointer will return an integer 
 * indicating the result of the comparison as follows:
 * *0 if the data1 and data2 comparison keys are equal.
 * *a negative value if data1 comparison key is less than data2 comparison key.
 * *a positive value if data1 comparison key is greater than data2 comparison 
 *  key.
 *
 * Return value:
 * bst_cmp_func_ty comparison function pointer will return an integer less than,
 * equal to, or greater than zero if data1 comparison key is found, 
 * respectively, to be less than, to match, or be greater than data2 comparison 
 * key.
 */
typedef int (*bst_cmp_func_ty)(const void *data1, const void *data2, 
								void *param);
/* DESCRIPTION:
 * bst_operation_func_ty operation function pointer is given by the user at 
 * BSTForeach() and is used by the user to perform some operation on data placed
 * within some range of bst nodes refered to by the appropriate iterators (see 
 * BSTForeach() for further information).
 * The operation performed must not change the values of any of the comparison 
 * keys.
 *
 * Return value:
 * bst_operation_func_ty operation function pointer will return 0 upon success.
 * If an error occure whithin the operation function, and the user wishes to 
 * terminate the BSTForeach() function before its inner loop is finnished, the 
 * operation function must return any integer other than 0 (see BSTForeach() for
 * further information).
 *
 * Warnings:
 * *It is required that operation_function return value will be 0 in case of 
 *  success. Any other return value will be interpeted by BSTForeach() function 
 *  as a sign that operation_function failed and will result in the terminating 
 *  of the function durring its run.
 * *operation_function must not change the value of the comparison key placed 
 *  inside each data. Such a thing will lead to undefind behavior.
 */
typedef int (*bst_operation_func_ty)(void *data, void *param);

/* DESCRIPTION:
 * BSTCreate() function create new bst data structure. The function receives
 * compare function pointer and param from user for with which the tree will be 
 * ordered (see bst_cmp_func_ty for compare function signiture specifications 
 * and also see BSTInsert() and BSTFind() for further information).
 * The function performs with complexity of O(1).
 * 
 * @param:
 * compare		pointer to comparison function.
 * param		helping parameter for compare function.
 *
 * Return value:
 * BSTCreate() function return a pointer to a new and empty bst data structure.
 * In case of malloc failure, NULL will be returned.
 *
 * Error:
 * If malloc fails, the function will return NULL.
 * 
 * Warning:
 * Do not place NULL as input of compare. Such a thing will lead to undefind 
 * behavior.
 */
bst_ty *BSTCreate(bst_cmp_func_ty compare, void *param);

/* DESCRIPTION:
 * BSTDestroy() function destroys given bst data structure.
 * The function performs with complexity of O(n) where n is the number of 
 * nodes placed in the tree.
 * 
 * @param:
 * bst			pointer to bst.
 *
 * Warning:
 * Do not place NULL as input of bst. Such a thing will lead to undefind 
 * behavior.
 */
void BSTDestroy(bst_ty *bst);

/* DESCRIPTION:
 * BSTSize() function return the number of nodes in given bst.
 * The function performs with complexity of O(n) where n is the number of 
 * nodes placed in the tree.
 * 
 * @param:
 * bst			pointer to bst
 *
 * Return value:
 * BSTSize() function return the number of nodes in given bst as a size_t 
 * integer.
 *
 * Warning:
 * Do not place NULL as input of bst. Such a thing will lead to undefind 
 * behavior.
 */
size_t BSTSize(const bst_ty *bst);

/* DESCRIPTION:
 * BSTIsEmpty() function checks whether given bst is empty or not.
 * The function performs with complexity of O(1).
 * 
 * @param:
 * bst			pointer to bst
 *
 * Return value:
 * BSTIsEmpty() function will return boolean TRUE if bst is empty, otherwise 
 * boolean FALSE.
 *
 * Warning:
 * Do not place NULL as input of bst. Such a thing will lead to undefind 
 * behavior.
 */
int BSTIsEmpty(const bst_ty *bst);

/* DESCRIPTION:
 * BSTInsert() function creates a new node and place data in it. Than, with the 
 * help of compare function given in BSTCreate() as compare function pointer and
 * than placed in bst, select the right place in the tree for the new node and 
 * connects it to bst.
 * The function performs with complexity of O(logn) where n is the number of 
 * nodes placed in the tree.
 * 
 * @param:
 * bst			pointer to bst
 * data			data to be inserted to bst
 *
 * Return value:
 * BSTInsert() function return an iterator of type iter_ty to the new inserted 
 * node.
 * In case of malloc failure, NULL will be returned.
 *
 * Error:
 * If malloc fails, the function will return NULL.
 *
 * Warnings:
 * *Do not place NULL as input of bst or of data. Such a thing will lead to 
 *  undefind behavior.
 * *This bst data structure relay on the fact that each key of data to be 
 *  inserted is unique and does not equal any other key already inserted to the
 *  tree. The behavior of two eaul keys of two different inserted data instanses
 *  is undefind.
 */
iter_ty BSTInsert(bst_ty *bst, const void *data);

/* DESCRIPTION:
 * BSTRemove() function removes the node pointed to by iter from the bst data 
 * structure.
 * The function performs with average complexity of O(logn) and worst case 
 * complexity of O(n) where n is the number of nodes placed in the tree.
 * 
 * @param:
 * iter			iterator pointing at node to be removed
 *
 * Warnings:
 * *Do not place NULL as input of iter. Such a thing will lead to undefind 
 *  behavior.
 * *Do not place value returned by or equal to the value returned by BSTEnd()
 *  as input of iter. Such a thing will lead to undefind behavior.
 */
void BSTRemove(iter_ty iter);

/* DESCRIPTION:
 * BSTBegin() function return an iterator pointed at the beginning of the bst, 
 * that is, the node containing the data with the smallest key value as sorted 
 * by the comparefunction given to bst in BSTCreate().
 * The function performs with complexity of O(logn) where n is the number of 
 * nodes placed in the tree.
 * 
 * @param:
 * bst			pointer to bst
 *
 * Return value:
 * BSTBegin() function return an iterator pointed at the beginning of the bst as
 * ordered.
 * If tree is empty, BSTEnd() value will be returned.
 * 
 * Warning:
 * Do not place NULL as input of bst. Such a thing will lead to undefind 
 * behavior.
 */
iter_ty BSTBegin(bst_ty *bst);

/* DESCRIPTION:
 * BSTEnd() function return an iterator pointed at the dummy node that defines 
 * the end of the bst data structure.
 * The function performs with complexity of O(1).
 * 
 * @param:
 * bst			pointer to bst
 *
 * Return value:
 * BSTEnd() function return an iterator pointed at the dummy node that defines 
 * the end of the bst data structure.
 * 
 * Warning:
 * Do not place NULL as input of bst. Such a thing will lead to undefind 
 * behavior.
 */
iter_ty BSTEnd(bst_ty *bst);

/* DESCRIPTION:
 * BSTPrev() function receives current iterator to node in bst and return an 
 * iterator to node that is previous to current in the order of sorting keys of 
 * inserted data, ordered by the compare function given as input in BSTCreate().
 * The function performs with complexity of O(logn) where n is the number of 
 * nodes placed in the tree.
 * 
 * @param:
 * current		iterator to node in bst data structure
 *
 * Return value:
 * BSTPrev() function return an iterator pointed to node that is previous to 
 * current in the order of bst data structure.
 * If current value is equal to iterator returned from BSTBegin() function, 
 * BSTEnd() value will be returned.
 * 
 * Warning:
 * Do not place NULL as input of current. Such a thing will lead to undefind 
 * behavior.
 */
iter_ty BSTPrev(iter_ty current);

/* DESCRIPTION:
 * BSTNext() function receives current iterator to node in bst and return an 
 * iterator to node that is next to current in the order of sorting keys of 
 * inserted data, ordered by the compare function given as input in BSTCreate().
 * The function performs with complexity of O(logn) where n is the number of 
 * nodes placed in the tree.
 * 
 * @param:
 * current		iterator to node in bst data structure
 *
 * Return value:
 * BSTNext() function return an iterator pointed to node that is next to current
 * in the order of bst data structure.
 * If current is pointed at the biggest key value in the bst data structure, 
 * according to the compare function given as input in BSTCreate(), BSTEnd() 
 * value will be returned.
 * 
 * Warning:
 * Do not place NULL as input of current. Such a thing will lead to undefind 
 * behavior.
 */
iter_ty BSTNext(iter_ty current);

/* DESCRIPTION:
 * BSTIterIsEqual() function compares iter1 to iter2 and return whether or not 
 * they are equal, that is, referring to the same node in bst data structure.
 * The function performs with complexity of O(1).
 * 
 * @param:
 * iter1		iterator to node in bst data structure
 * iter2		iterator to node in bst data structure
 *
 * Return value:
 * BSTIterIsEqual() function return boolean TRUE if iter1 is equal to iter2, and
 * boolean FALSE if not.
 * 
 * Warning:
 * Do not place NULL as input of iter1 or iter2. Such a thing will lead to 
 * undefind behavior.
 */
int BSTIterIsEqual(iter_ty iter1, iter_ty iter2);

/* DESCRIPTION:
 * BSTGetData() function return the data placed inside the node referred to by 
 * iter.
 * The function performs with complexity of O(1).
 * 
 * @param:
 * iter			iterator to node in bst data structure
 *
 * Return value:
 * BSTGetData() function return the data placed inside the node referred to by 
 * iter.
 * 
 * Warning:
 * Do not place NULL as input of iter. Such a thing will lead to undefind 
 * behavior.
 */
void *BSTGetData(iter_ty iter);

/* DESCRIPTION:
 * BSTFind() function receives bst data structure and data to search in bst and
 * return an iterator referring to node containing said data if found.
 * The function performs with complexity of O(n) where n is the number of nodes 
 * placed in the tree.
 * 
 * @param:
 * bst			pointer to bst
 * data			data to search in bst data structure nodes
 *
 * Return value:
 * BSTFind() function return an iterator referring to node containing said data 
 * if found.
 * If data is not found in bst, BSTEnd() value will be returned.
 * 
 * Warning:
 * Do not place NULL as input of bst or data. Such a thing will lead to 
 * undefind behavior.
 */
iter_ty BSTFind(bst_ty *bst, const void *data);

/* DESCRIPTION:
 * BSTForeach() function receives range of nodes of from (included) until to 
 * (excluded) to send to operated by operation_function with param (see typedef 
 * of bst_operation_func_ty for further details on the correct function 
 * signiture). If an error ocures in operation_function durring the run, the 
 * fuction will stop its inner loop and exit with the returned value given by 
 * operation_function (see Warnings for further detail).
 * The function performs with complexity of O(n) where n is the number of nodes 
 * placed between from (included) and to (excluded).
 * 
 * @param:
 * from						iterator to node in bst data structure
 * to						iterator to node in bst data structure
 * operation_function		pointer to operation function.
 * param					helping parameter for operation function.
 *
 * Return value:
 * BSTForeach() function return 0 if operation on each node in the range of from
 * (included) until to (excluded) is successfull.
 * If an error ocures in operation_function durring the run, the fuction will 
 * stop its inner loop and exit with the returned value given by 
 * operation_function.
 * 
 * Warnings:
 * *Do not place NULL as input of from, to or operation_function. Such a thing 
 *  will lead to undefind behavior.
 * *It is required that operation_function return value will be 0 in case of 
 *  success. Any other return value will be interpeted by BSTForeach() function 
 *  as a sign that operation_function failed and will result in the terminating 
 *  of the function durring its run.
 * *operation_function must not change the value of the comparison key placed 
 *  inside each data. Such a thing will lead to undefind behavior.
 */
int BSTForeach(iter_ty from, iter_ty to,
				bst_operation_func_ty operation_function, void *param);

#endif /* __BST_H__ */
