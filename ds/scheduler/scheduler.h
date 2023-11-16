#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>

#include "uid.h"

typedef struct scheduler scheduler_t;

/* DESCRIPTION:
 * SchedulerCreate() function creates a new empty scheduler.
 * 
 * Return value:
 * scheduler_t pointer to the new scheduler.
 * NULL if an error occure.
 *
 * Error:
 * The SchedulerCreate() function will return NOT_ASSIGNED if there is 
 * insufficient memory available to allocate new scheduler.
 */
scheduler_t *SchedulerCreate();

/* DESCRIPTION:
 * SchedulerDestroy() function destroys given scheduler.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 */
void SchedulerDestroy(scheduler_t *scheduler);

/* DESCRIPTION:
 * SchedulerExecute() function execute scheduler tasks at their timely manner
 * and in a loop. Unless an error occure, the function will continue untile the
 * scheduler is turned off with SchedulerStop() function.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 *
 * Return value:
 * *SUCCESS for successfully executing all given tasks until SchedulerStop() 
 *  function is invoked.
 * *FAIL if an error ocure (see Errors section for possible errors).
 *
 * Errors:
 * The SchedulerExecute() function will return an error if one of the following 
 * instances occurs:
 * *Insufficient memory available to allocate current task.
 * *The scheduler is empty but SchedulerStop() function was not invoked.
 */
int SchedulerExecute(scheduler_t *scheduler);

/* DESCRIPTION:
 * SchedulerClear() function clears all the tasks in the scheduler.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 */
void SchedulerClear(scheduler_t *scheduler);

/* DESCRIPTION:
 * SchedulerInsertTask() function creates task with user defined op_func, 
 * which will receive func_params and will be executed according to the interval
 * time when task is invoked in the SchedulerExecute() function.
 * In addition, a UID will be generated for the task.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 * op_func		user defined function
 * func_params	parameters for user defined function
 * interval		time interval to perform the task (in seconds)
 *
 * Return value:
 * If SchedulerInsertTask() function succseeds, it will return the uid of the 
 * inserted task.
 * If fails, it will return a bad uid (see Error section for possible errors).
 *
 * Error:
 * The SchedulerInsertTask() function will return an error if there is 
 * insufficient memory available to allocate the new task to.
 */
ilrd_uid_t SchedulerInsertTask(scheduler_t *scheduler,
					int (*op_func)(void *func_params), 
									void *func_params, 
									size_t interval);

/* DESCRIPTION:
 * SchedulerRemoveTask() function removes the task that match the uid parameter 
 * from the scheduler.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 * uid			the requested task's uid
 *
 * Return value:
 * SchedulerRemoveTask() function will return:
 * SUCCESS for successfully removing the requested task.
 * FAIL if the given uid is not found.
 */
int SchedulerRemoveTask(scheduler_t *scheduler, ilrd_uid_t uid);

/* DESCRIPTION:
 * SchedulerSize() function return the number of tasks in given scheduler.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 *
 * Return value:
 * SchedulerSize() function return the number of tasks as a size_t integer.
 */
size_t SchedulerSize(scheduler_t *scheduler);

/* DESCRIPTION:
 * SchedulerIsEmpty() function checks whether given scheduler is empty or not.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 *
 * Return value:
 * SchedulerIsEmpty() function will return boolean TRUE if scheduler is empty, 
 * otherwise boolean FALSE.
 */
int SchedulerIsEmpty(scheduler_t *scheduler);

/* DESCRIPTION:
 * SchedulerStop() function stops the SchedulerExecute() function loop. When 
 * provoked, the SchedulerExecute() function will exit successfuly from its 
 * inner loop after finnishing its current task (not before).
 * 
 * @param:
 * scheduler	pointer to a scheduler
 */
void SchedulerStop(scheduler_t *scheduler);

/* DESCRIPTION:
 * SchedulerIsActive() function checks wether scheduler is active or not.
 * 
 * @param:
 * scheduler	pointer to a scheduler
 *
 * Return value:
 * SchedulerIsActive() function will return boolean TRUE if scheduler is active, 
 * otherwise boolean FALSE.
 */
int SchedulerIsActive(scheduler_t *scheduler);

#endif /*__SCHEDULER_H__*/

