#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef struct slist slist_t;
typedef struct node node_t;
typedef node_t *iterator_t;

typedef int (*match_func_t)(const void *data, void *param);
typedef int (*operation_func_t)(void *data, void *param); 


/* DESCRIPTION:
 * Create the slist struct and two  dummy node head and tail  
 * 
 * @param:void
 * 
 *
 * @return:
 * pointer to new slist struct 
 */
slist_t *SListCreate();

/* DESCRIPTION:
 * Destroy the slist struct and the dummy node
 *
 * @param:
 * slist    pointer to slist
 *
 * @return: 
 * ***    void
 */
void SListDestroy(slist_t *slist);

/* DESCRIPTION:
 * check whether the slist is empty or not
 *
 * @param:
 * slist    pointer to slist
 *
 * @return:
 * 1 if the slist is empty otherwise 0
 */
int SListIsEmptyList(const slist_t *slist);

/* DESCRIPTION:
 * Get the value from position of iterator 
 *
 * @param:
 * iter  itertor 
 *
 * @return:
 * value of iterator current position
 */
void *SListGetData(const iterator_t iter);


/* DESCRIPTION:
 * set the value of iterator current position
 *
 * @param:
 * iter  itertor
 * data  value for update

 * @return:
 *    void
 */
void SListSetData(iterator_t iter, const void *data);


/* DESCRIPTION:
 * find specific node according to user match function
 * in range from to to(not including the to)
 * 
 * @param:
 * from- begin of range
 * to- end of range
 * pointer to is match function
 * param user value
 *
 * @return:
 * iterator to the find node
 */
iterator_t SListFindInSlist(iterator_t from, iterator_t to, 
							match_func_t is_match, void *param);


/* DESCRIPTION:
 *  insert the next value in iterator place
 *
 * @param:
 * iterator_t iterator, 
 * void* data to enter
 *
 * @return:
 * update iterator 
 */
iterator_t SListInsert(iterator_t where, const void *data);


/* DESCRIPTION:
 * remove the next value in iterator place
 *
 * @param:
 * iterator_t iterator
 *
 * @return:
 * update iterator
 */
iterator_t SListRemove(iterator_t iter);


/* DESCRIPTION:
 * count the num of nodes in the slist
 *
 * @param:
 * slist    pointer to slist
 *
 * @return:
 * size_t num of nodes
 */
size_t SListCount(const slist_t *slist);

/* DESCRIPTION:
 * make specific operation of each node according 
 * to user is_operated function
 * in range from to to(not including the to)
 * 
 * @param:
 * from- begin of range
 * to- end of range
 * pointer to is is_operated
 * param user value
 *
 * @return:
 * 0 if succeded, otherwise 0
 */
int SListForeach(iterator_t from, iterator_t to, operation_func_t is_operated, 
					void *param);

/***Iterators Functions***/

/* DESCRIPTION:
 * move iterator to the last node
 *
 * @param:
 * slist    pointer to slist
 *
 * @return:
 * update iterator
 */
iterator_t SListEnd(const slist_t *slist);


/* DESCRIPTION:
 * move iterator to the first node
 *
 * @param:
 * slist    pointer to slist
 *
 * @return:
 * update iterator
 */
iterator_t SListBegin(const slist_t *slist);

/* DESCRIPTION:
 *	move iterator to the next node 
 *  can't move the end iterator 
 *
 * @param:
 * iter  itertor 
 *
 * @return:
 * update iterator- to next value
 */
iterator_t SListNext(const iterator_t current);

/* DESCRIPTION:
 * check if both point to same place
 *
 * @param:
 * iter1 first iterator
 * iter2 second iterator
 * 
 *
 * @return:
 *  1 if equal and 0 if arent equal
 * 
 */
int SListIsEqual(const iterator_t iter1, const iterator_t iter2);

/******************************************************************************/
/* For queue exercise: */

/* DESCRIPTION:
 * Apends beginning of src slist to end of dst slist, than points src->head to
 * src->tail.
 *
 * @param:
 * dst dst slist iterator
 * src src slist iterator
 */
void SListAppend(slist_t *dst, slist_t *src);

#endif /* __SLIST_H__ */


