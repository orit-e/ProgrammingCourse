#ifndef __UID_H__
#define __UID_H__

#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>


/* Might change in future. User can't relly on current stract build.*/
typedef struct uid
{
	pthread_t thread_id;
	pid_t pid;	
	time_t time_stamp;
	size_t counter;/*static variable in create function*/
}ilrd_uid_t;


/* DESCRIPTION:
 * Create new UID 
 * 			
 * @return:
 * UID 
 */
ilrd_uid_t UIDCreate(void);

/* DESCRIPTION:
 * Create bad UID 
 * 
 * @return:
 * bad UID
 */
ilrd_uid_t BadUIDCreate(void);

/* DESCRIPTION:
 * Compares two uids
 *
 * @param:	
 * uid1, uid2    UIDs to be compared
 *
 * @return:
 * 1 if equal, 0 if not
 */
int UIDISEqual(const ilrd_uid_t *uid1, const ilrd_uid_t *uid2);

#endif /* __UID_H__ */

