/******************************************************************************
 											
				File name:			hash.h			
				Written by:     	Orit			
				Reviewed by:    	Elia
 											
 ******************************************************************************/
#ifndef __HASH__H__
#define __HASH__H__

#include <stddef.h> 		/* size_t */

typedef struct hash hash_ty;

/**
 * @DESCRIPTION:
 * hash_func_ty hash function pointer is given by the user at HashCreate() and 
 * is used by the user to determine the right hash table index for the given 
 * key of data (see HashCreate(), HashRemove() and HashFind() for further 
 * information).
 *
 * @RETURN:
 * hash_operation_func_ty operation function pointer will return the right hash 
 * table index for the given key of data.
 *
 * @WARNING:
 * For a faster and more efficient hash table, the user should aim to create a 
 * unique hash table index for each unique data key.
 */
typedef size_t (*hash_func_ty)(const void *data);

/**
 * @DESCRIPTION:
 * hash_operation_func_ty operation function pointer is given by the user at 
 * HashForEach() and is used by the user to perform some operation on values 
 * placed within the data in the hash (see HashForEach() for further 
 * information).
 * The operation performed must not change the values of any of the comparison 
 * keys.
 *
 * @RETURN:
 * hash_operation_func_ty operation function pointer will return 0 upon success.
 * If an error occure whithin the operation function, and the user wishes to 
 * terminate the HashForEach() function before its inner loop is finnished, the 
 * operation function must return any integer other than 0 (see HashForEach() 
 * for further information).
 *
 * @WARNINGS:
 * *It is required that the operation function return value will be 0 in case 
 *  of success. Any other return value will be interpeted by HashForEach() 
 *  function as a sign that the operation function failed and will result in 
 *  the terminating of the function durring its run.
 * *the operation function must not change the value of the comparison key 
 *  placed inside each data. Such a thing will lead to undefind behavior.
 */
typedef int(*hash_operation_func_ty)(void *data, void *op_param);

/** 
 * @DESCRIPTION:
 * hash_cmp_func_ty comparison function pointer is given by the user at 
 * HashCreate() and is used by hash functions HashRemove() and HashFind() to 
 * find wanted data through key comparison. The comparison function will 
 * receive and compare data1 and data2 keys. hash_cmp_func_ty comparison 
 * function pointer will return an integer indicating the result of the 
 * comparison as follows:
 * *0 if the data1 and data2 comparison keys are equal.
 * *a negative value if data1 comparison key is of lesser priority than data2 
 *  comparison key.
 * *a positive value if data1 comparison key is of greater priority than data2 
 *  comparison key.
 *
 * @RETURN:
 * hash_cmp_func_ty comparison function pointer will return an integer less 
 * than, equal to, or greater than zero if data1 comparison key is found, 
 * respectively, to be of lesser, equal, or of greater priority than data2 
 * comparison key.
 */
typedef int (*hash_cmp_func_ty)(const void *data1, const void *data2);

/**
 * @DESCRIPTION:
 * HashCreate() function creates a new hash. The function receives a hash_func 
 * and compare function pointers and hash_range from the user (see 
 * hash_cmp_func_ty and hash_func_ty functions signature specifications and 
 * also see HashRemove() and HashFind() for further information on the compare 
 * function).
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * hash_func		pointer to hash function.
 * compare			pointer to comparison function.
 * hash_range		determine the size of the hash table.
 *
 * @RETURN:
 * HashCreate() function returns a pointer to a new and empty hash.
 * In case of malloc failure, NULL will be returned.
 *
 * @ERROR:
 * If malloc fails, the function will return NULL.
 * 
 * @WARNINGS:
 * *Do not place NULL as input of hash_func or compare. Such a thing will lead 
 *  to undefined behavior.
 * *Do not place 0 as input of hash_range. Such a thing will lead to undefined 
 *  behavior.
 */
hash_ty *HashCreate(hash_func_ty hash_func, size_t hash_range, 
                                          hash_cmp_func_ty compare);

/**
 * @DESCRIPTION:
 * HashDestroy() function destroys given hash structure.
 * The function performs with time complexity of O(n + k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * 
 * @PARAM:
 * hash				pointer to hash data structure.
 *
 * @WARNING:
 * Do not place NULL as input of hash. Such a thing will lead to undefined 
 * behavior.
 */
void HashDestroy(hash_ty *hash);

/**
 * @DESCRIPTION:
 * HashInsert() function receives data to place in the hash and then, with the 
 * help of the hash_func function given in HeapCreate(), select the right place 
 * in the hash for it to be inserted.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * hash				pointer to hash data structure.
 * data				data to be inserted to the hash.
 *
 * @RETURN:
 * HashInsert() function return 0 for success and 1 otherwise.
 *
 * @WARNING:
 * Do not place NULL as input of hash or data. Such a thing will lead to 
 * undefined behavior.
 */
int HashInsert(hash_ty *hash, void *data);

/**
 * @DESCRIPTION:
 * HashRemove() function removes from the heap the data element with key equal 
 * to that of data input variable as compared through the compare function 
 * given in HashCreate().
 * The function performs with time complexity of O(n + k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * (See HashCreate() function and hash_func_ty and hash_cmp_func_ty function 
 * pointers signatures for further information).
 * 
 * @PARAM:
 * hash				pointer to hash data structure.
 * data				data with the right key to be removed from the hash.
 *
 * @WARNINGS:
 * *Do not place NULL as input of hash. Such a thing will lead to undefined 
 *  behavior.
 */
void HashRemove(hash_ty *hash, const void *data);

/**
 * @DESCRIPTION:
 * HashSize() function returns the number of data elements in given hash.
 * The function performs with time complexity of O(n + k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * 
 * @PARAM:
 * hash				pointer to hash data structure.
 *
 * @RETURN:
 * HashSize() function returns the number of data elemnts in given hash.
 *
 * @WARNING:
 * Do not place NULL as input of hash. Such a thing will lead to undefined 
 * behavior.
 */
size_t HashSize(const hash_ty *hash);

/**
 * @DESCRIPTION:
 * HashIsEmpty() function checks whether given hash is empty or not.
 * The function performs with time complexity of O(n) where n is the hash_range
 * given by the user in HashCreate().
 * 
 * @PARAM:
 * hash				pointer to hash data structure.
 *
 * @RETURN:
 * HashIsEmpty() function will return 1 if heap is empty, otherwise 0.
 *
 * @WARNING:
 * Do not place NULL as input of hash. Such a thing will lead to undefined 
 * behavior.
 */
int HashIsEmpty(const hash_ty *hash);

/**
 * @DESCRIPTION:
 * HashFind() function finds in the heap the data element with key equal 
 * to that of data input variable as compared through the compare function 
 * given in HashCreate(). If found, the function will return the found data to 
 * the user as void pointer.
 * The function performs with time complexity of O(n + k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * (See HashCreate() function and hash_func_ty and hash_cmp_func_ty function 
 * pointers signatures for further information).
 * 
 * @PARAM:
 * hash			pointer to hash data structure.
 * data			data with the right key to be searched for in the hash.
 *
 * @RETURN:
 * HashFind() function return a pointer to searched data if found.
 * If data is not found, NULL will be returned.
 * 
 * @WARNINGS:
 * Do not place NULL as input of hash or data. Such a thing will lead to 
 * undefind behavior.
 */
void *HashFind(const hash_ty *hash, const void *data_to_find);

/**
 * @DESCRIPTION:
 * HashForEach() function sends all data inserted to hash to be operated by 
 * operation function with op_param (see typedef of hash_operation_func_ty for 
 * further details on the correct function signiture). If an error occurs in 
 * operation function in mid run, the fuction will stop its inner loop and 
 * exit with the returned value given by operation function (see WARNINGS for 
 * further detail).
 * The function performs with complexity of O(n * k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * 
 * @PARAM:
 * hash			pointer to hash data structure.
 * operation	pointer to operation function.
 * op_param		helping parameter for operation function.
 *
 * @RETURN:
 * HashForEach() function return 0 if operation on each data element was 
 * successfull.
 * If an error ocures in operation function in mid run, the fuction will stop 
 * its inner loop and exit with the returned value given by operation 
 * function.
 * 
 * @WARNINGS:
 * *Do not place NULL as input of hash or operation. Such a thing will lead to 
 *  undefined behavior.
 * *It is required that operation function return value will be 0 in case of 
 *  success. Any other return value will be interpeted by HashForEach() 
 *  function as a sign that operation function failed and will result in the 
 *  terminating of the function in mid run.
 * *operation function must not change the value of the comparison key placed 
 *  inside each data. Such a thing will lead to undefined behavior.
 */
int HashForEach(hash_ty *hash, hash_operation_func_ty operation, 
                void *op_param);


/**
 * @DESCRIPTION:
 * HashLoad() function calculate the load factor of given hash.
 * The function performs with complexity of O(n + k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * 
 * @PARAM:
 * hash			pointer to hash data structure.
 *
 * @RETURN:
 * HashLoad() function return the load factor of given hash as double 
 * variable.
 * 
 * @WARNING:
 * Do not place NULL as input of hash. Such a thing will lead to undefined 
 * behavior.
 */
double HashLoad(const hash_ty *hash);

/**
 * @DESCRIPTION:
 * HashStdev() function calculate the standard deviation of given hash. The 
 * function uses functions from math.h standard library.
 * The function performs with complexity of O(n * k) where n is hash_range 
 * given by user in HashCreate() and k is the largest number of data elements 
 * placed in one index the hash.
 * 
 * @PARAM:
 * hash			pointer to hash data structure.
 *
 * @RETURN:
 * HashStdev() function return the standard deviation of given hash as double 
 * variable.
 * 
 * @WARNING:
 * Do not place NULL as input of hash. Such a thing will lead to undefined 
 * behavior.
 */
double HashStdev(const hash_ty *hash);


#endif /* __HASH_H__ */