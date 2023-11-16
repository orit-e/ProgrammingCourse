#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include "../dlist/dlist.h"

typedef struct sorted_list sorted_list_t;
typedef int(*sorted_list_match_func_t)(const void *data, const void *param);
typedef int(*sorted_list_operation_func_t)(void *data, void *param);

/* DESCRIPTION:
 * compare two pointers 
 * 
 * @param:
 * data1, data2		two pointers to objects of same type		
 *
 * @return:
 * positive value if data1 is greater than data2,
 * negative value if data2 is greater than data1,
 * 0  it data1 and data2 are equal 
 */
typedef int(*sorted_list_cmp_func_t)(const void *data1, const void *data2);

/* DESCRIPTION:
 * create sorted list struct and two dummy nodes - head and tail.
 * get compare function for sort new nodes. 
 * 
 * @param:
 * cmp		function that compare two node->data
 * @return:
 * pointer to new sorted list struct 
 */
sorted_list_t *SortedListCreate(sorted_list_cmp_func_t cmp);

/* DESCRIPTION:
 * destroy the list struct and the dummy nodes
 *
 * @param:	
 * dlist    pointer to sorted list
 */
void SortedListDestroy(sorted_list_t *sorted_list);

/* DESCRIPTION:
 * check whether the list is empty or not
 *
 * @param:
 * sorted_list    pointer to sorted list
 *
 * @return:
 * 1 if the list is empty, otherwise 0
 */
int SortedListIsEmpty(const sorted_list_t *sorted_list);

/* DESCRIPTION:
 * Get data from iterator position 
 *
 * @param:
 * iter  itertor 
 *
 * @return:
 * pointer to node data pointed to by iter
 */
void *SortedListGetData(const iterator_t iter);


/* DESCRIPTION:
 * insert new node with given data to sorted list
 *
 * @param:
 * sorted_list	sorted list
 * data			the data for the new node
 *
 * @return:
 * iterator pointed to the new inserted node
 */
iterator_t SortedListInsert(sorted_list_t *sorted_list, const void *data);

/* DESCRIPTION:
 * remove node pointed to by iter....for remove end of list use popBack function
 *
 * @param:
 * iter		iterator to target node
 *
 * @return:
 * iterator to the node after the removed node
 */
iterator_t SortedListRemove(iterator_t iter);

/* DESCRIPTION:
 * count the nodes in the list
 *
 * @param:
 * sorted_list    pointer to sorted list
 *
 * @return:
 * num of nodes
 */
size_t SortedListCount(const sorted_list_t *sorted_list);

/* DESCRIPTION:
 * return the data of the first node and removing it from the list
 *
 * @param:
 * sorted_list		sorted list
 *
 * @return:
 * pointer to the data
 */
void *SortedListPopFront(sorted_list_t *sorted_list);

/* DESCRIPTION:
 * return the data of the last node and removing it from the list
 *
 * @param:
 * sorted_list		sorted list
 *
 * @return:
 * pointer to the data
 */
void *SortedListPopBack(sorted_list_t *sorted_list);

/* DESCRIPTION:
 * find specific node according to user match function
 * in specific range
 * 
 * @param:
 * from		begin of range
 * to		end of range (exclude)
 * is_match	pointer to is match function
 * param	user value
 *
 * @return:
 * iterator to the find node, if data not found return tail 
 */
iterator_t SortedListFindIf(iterator_t from,
							iterator_t to,
							sorted_list_match_func_t is_match,
							const void *param);

/* DESCRIPTION:
 * find node that contain specific data in specific range
 * 
 * @param:
 * from		begin of range
 * to		end of range (exclude)
 * data		data to find node that contain it
 *
 * @return:
 * iterator to the found node, if data not found return tail 
 */
iterator_t SortedListFind(iterator_t from,
						  iterator_t to,
						  const void *data,
						  sorted_list_t *sorted_list);

/* DESCRIPTION:
 * make specific operation of each node according 
 * to user defined is_operated function
 * in range from to to (not including the to)
 * 
 * @param:
 * from		begin of range
 * to		end of range
 * is_operated	pointer to user defined function
 * param 	user value
 *
 * @return:
 * 0 if succeded, otherwise 1
 */
int SortedListForeach(iterator_t from,
					  iterator_t to,
					  sorted_list_operation_func_t is_operated,
					  const void *param);

/* DESCRIPTION:
 * marge two sorted lists into one sorted list
 * 
 * @param:
 * dst		the list that will contain the two sorted lists
 * src		the list that will be merged to dst.
 *          src list will remain empty.
 */
void SortedListMerge(sorted_list_t *dst, sorted_list_t *src);


/***********Iterators Functions**************/

/* DESCRIPTION:
 * move iterator to the last node [dummy tail]
 *
 * @param:
 * sorted_list    pointer to sorted_list
 *
 * @return:
 * iterator to dummy tail
 */
iterator_t SortedListEnd(const sorted_list_t *sorted_list);

/* DESCRIPTION:
 * get iterator to the first node
 *
 * @param:
 * sorted_list    sorted list
 *
 * @return:
 * iterator to first node in sorted_list
 */
iterator_t SortedListBegin(const sorted_list_t *sorted_list);

/* DESCRIPTION:
 * get iterator of the next node 
 *
 * @param:
 * current		iterator 
 *
 * @return:
 * iterator to the next node
 */
iterator_t SortedListNext(const iterator_t current);

/* DESCRIPTION:
 * get iterator of the prev node 
 *
 * @param:
 * current		iterator 
 *
 * @return:
 * iterator to prev node
 */
iterator_t SortedListPrev(const iterator_t current);

/* DESCRIPTION:
 * check if both iterators are equal
 *
 * @param:
 * iter1	first iterator
 * iter2	second iterator
 * 
 * @return:
 * 1 if equal and 0 if not
 */
int SortedListIsEqual(const iterator_t iter1, const iterator_t iter2);

#endif /* __SORTED_LIST_H__ */
