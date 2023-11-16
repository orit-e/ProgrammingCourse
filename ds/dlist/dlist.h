#ifndef __DLIST_H__
#define __DLIST_H__

typedef struct dlist dlist_t;
typedef struct node node_t;
typedef node_t *iterator_t;
typedef int(*match_func_t)(const void *data,const void *param);
typedef int(*operation_func_t)(void *data, void *param);

/* DESCRIPTION:
 * create the dlist struct and two dummy nodes - head and tail  
 * 
 * @return:
 * pointer to new dlist struct 
 */
dlist_t *DListCreate();

/* DESCRIPTION:
 * destroy the dlist struct and the dummy nodes
 *
 * @param:
 * dlist    pointer to dlist
 */
void DListDestroy(dlist_t *dlist);

/* DESCRIPTION:
 * check whether the dlist is empty or not
 *
 * @param:
 * dlist    pointer to dlist
 *
 * @return:
 * 1 if the dlist is empty otherwise 0
 */
int DListIsEmpty(const dlist_t *dlist);

/* DESCRIPTION:
 * Get the value from position of iterator 
 *
 * @param:
 * iter  itertor 
 *
 * @return:
 * pointer to value of node pointed to by iterator
 */
void *DListGetData(const iterator_t iter);

/* DESCRIPTION:
 * set value of node pointed to by iterator
 *
 * @param:
 * iter  itertor
 * data  value for update
 */
void DListSetData(iterator_t iter, const void *data);

/* DESCRIPTION:
 * insert new node with given value before iterator place
 *
 * @param:
 * where	iterator, 
 * date		value to enter
 *
 * @return:
 * iterator to inserted node
 */
iterator_t DListInsertBefore(iterator_t where, const void *data);

/* DESCRIPTION:
 * insert new node with given value after iterator place
 *
 * @param:
 * where	iterator, 
 * date		value to enter
 *
 * @return:
 * iterator to inserted node
 */
iterator_t DListInsertAfter(iterator_t where, const void *data);

/* DESCRIPTION:
 * remove node pointed to by iter, removing DListEnd results in undefined behaviour
 *
 * @param:
 * iter		iterator to target node
 *
 * @return:
 * iterator to the node after the removed node
 */
iterator_t DListRemove(iterator_t iter);

/* DESCRIPTION:
 * count the num of nodes in the dlist
 *
 * @param:
 * dlist    pointer to dlist
 *
 * @return:
 * num of nodes
 */
size_t DListCount(const dlist_t *dlist);

/* DESCRIPTION:
 * insert new node with given value at list begining
 *
 * @param:
 * dlist	list to add new node to
 * data		value to add
 *
 * @return:
 * iterator to inserted node
 */
iterator_t DListPushFront(dlist_t *dlist, const void *data);

/* DESCRIPTION:
 * insert new node with given value at list end
 *
 * @param:
 * dlist	list to add new node to
 * data		value to add
 *
 * @return:
 * iterator to inserted node
 */
iterator_t DListPushBack(dlist_t *dlist, const void *data);

/* DESCRIPTION:
 * remove node from list beginning
 *
 * @param:
 * dlist	list to remove node from
 *
 * @return:
 * pointer to data of removed node
 */
void *DListPopFront(dlist_t *dlist);

/* DESCRIPTION:
 * remove node from list end
 *
 * @param:
 * dlist	list to remove node from
 *
 * @return:
 * pointer to data of removed node
 */
void *DListPopBack(dlist_t *dlist);

/* DESCRIPTION:
 * given two different lists, append before 'where' of first list 
 * nodes in range 'from' - 'to' [non inclusive] from second list. 
 * nodes in range will be taken out of second list to first list.
 *
 * @param:
 * where	iterator to first list
 * from		begining of range in second list
 * to		end if range in second list
 *
 * @return:
 * iterator to 'where'
 */
iterator_t DListSplice(iterator_t where, iterator_t from, iterator_t to);

/* DESCRIPTION:
 * find specific node according to user match function
 * in range from to to (not including the to)
 * 
 * @param:
 * from		begin of range
 * to		end of range
 * is_match	pointer to is match function
 * param	user value
 *
 * @return:
 * iterator to the find node
 */
iterator_t DListFind(iterator_t from, iterator_t to, match_func_t is_match, const void *param);

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
 * 0 if succeded, otherwise a different integer.
 */
int DListForeach(iterator_t from, iterator_t to, operation_func_t is_operated, const void *param);

/*ADVANCED:*/
/* DESCRIPTION:
 * find specific node in one list according to user match function
 * in range from to to (not including the to).
 * PushBack a copy of found value to output_list.
 * 
 * @param:
 * from		begin of range
 * to		end of range
 * is_match 	pointer to is match function
 * param	user value
 *
 * @return:
 * 0 if succeded, otherwise 1.
 */
int DListMultiFind(iterator_t from, iterator_t to, match_func_t is_match, void const *param, dlist_t *output_list);/**/


/***********Iterators Functions**************/

/* DESCRIPTION:
 * move iterator to the last node [dummy tail]
 *
 * @param:
 * dlist    pointer to dlist
 *
 * @return:
 * iterator to dummy tail
 */
iterator_t DListEnd(const dlist_t *dlist);


/* DESCRIPTION:
 * move iterator to the first node
 *
 * @param:
 * dlist    pointer to dlist
 *
 * @return:
 * iterator to first node in list
 */
iterator_t DListBegin(const dlist_t *dlist);

/* DESCRIPTION:
 * move iterator to the next node 
 *
 * @param:
 * current	iterator 
 *
 * @return:
 * iterator to next node
 */
iterator_t DListNext(const iterator_t current);

/* DESCRIPTION:
 * move iterator to the prev node 
 *
 * @param:
 * current	iterator 
 *
 * @return:
 * iterator to prev node
 */
iterator_t DListPrev(const iterator_t current);

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
int DListIsEqual(const iterator_t iter1, const iterator_t iter2);

#endif /* dlist.h */
