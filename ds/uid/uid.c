/********************************************
 *											*
 *		filename:			dlist.c			*
 *		written by:     	Orit			*
 *		reviewed by:    	Yona			*
 *		last edit:      	10.08.2020		*
 *											*
 ********************************************/
#include <assert.h>		/*	assert()	*/
#include <stdatomic.h>	/*	ATOMIC_VAR_INIT(), atomic_fetch_add(), 
							atomic_load(), atomic_size_t			*/
#include "uid.h"

enum BAD_ILRD_UID
{
	BAD_UNSIGNED = -1,
	BAD_SIGNED = 0
};

ilrd_uid_t UIDCreate()
{
	static atomic_size_t counter = ATOMIC_VAR_INIT(1);
	ilrd_uid_t new_uid;
	
	new_uid.pid = getpid();
	new_uid.thread_id = pthread_self();
	new_uid.time_stamp = time(NULL);
	new_uid.counter = atomic_fetch_add(&counter, 1);
	
	return new_uid;
}

ilrd_uid_t BadUIDCreate()
{
	ilrd_uid_t bad_uid = {BAD_SIGNED, BAD_UNSIGNED, BAD_UNSIGNED, BAD_SIGNED};
	
	return bad_uid;
}

int UIDISEqual(const ilrd_uid_t *uid1, const ilrd_uid_t *uid2)
{
	assert(NULL != uid1);
	assert(NULL != uid2);

	return (uid1->pid == uid2->pid && 
			0 != pthread_equal(uid1->thread_id, uid2->thread_id) &&
			uid1->time_stamp == uid2->time_stamp &&
			atomic_load(&uid1->counter) == atomic_load(&uid2->counter));
}
