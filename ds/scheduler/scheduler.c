/********************************************
 *											*
 *		filename:			scheduler.c		*
 *		written by:     	Orit			*
 *		reviewed by:    	Dor				*
 *											*
 ********************************************/
#include <stdlib.h>		/*	malloc(), free(), size_t	*/
#include <string.h>		/*	memset()					*/
#include <assert.h>		/*	assert()					*/
#include <stdatomic.h>	/*	atomic_load(), atomic_int	*/

#include "scheduler.h"
#include "priority_queue.h"
#include "task.h"

/******************************************************************************/
/*					enum and structs and typedefs declarations:				  */
#define NOT_ASSIGNED NULL

enum ACTIVE_OR_NOT_ACTIVE
{
	NOT_ACTIVE = 0, 
	ACTIVE
};

enum FAIL_OR_SUCCESS
{
	SUCCESS = 0, 
	FAIL
};

enum IS_EMPTY
{
	YES_EMPTY = 1
};

enum IS_UID_EQUAL
{
	UID_IS_EQUAL = 1
};
enum EXECUTION_TIME_TO_CURRENT_TIME
{
	EXECUTE_NOW = 0
};

struct scheduler
{
	priority_q_t *pq;
	task_t *current_task;
	atomic_int is_active;  	
};

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
static int SchedulerTimeDiffIMP(time_t time1, time_t time2);
static int SchedulerCMPIMP(const void *data1, const void *data2, void *param);
static int SchedulerIsMAtchUIDIMP(const void *data1, const void *data2);
static task_t *GetCurrentTaskIMP(scheduler_t *scheduler);
static void SetCurrentTaskIMP(scheduler_t *scheduler, task_t *task);

/******************************************************************************/
/*								Scheduler functions:						  */
scheduler_t *SchedulerCreate()
{
	scheduler_t *new_scheduler = (scheduler_t *)malloc(sizeof(*new_scheduler));
	
	if(NOT_ASSIGNED == new_scheduler)
	{
		return NOT_ASSIGNED;
	}
	memset(new_scheduler, 0, sizeof(*new_scheduler));
	new_scheduler->pq = PQCreate(SchedulerCMPIMP, NULL);
	if(NOT_ASSIGNED == new_scheduler->pq)
	{
		free(new_scheduler);
		return NOT_ASSIGNED;
	}
	SetCurrentTaskIMP(new_scheduler, NOT_ASSIGNED);
	new_scheduler->is_active = ACTIVE;
	
	return new_scheduler;
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	
	SchedulerClear(scheduler);
	PQDestroy(scheduler->pq);
	memset(scheduler, 0, sizeof(*scheduler));
	free(scheduler);
	scheduler = NOT_ASSIGNED;
}

int SchedulerExecute(scheduler_t *scheduler)
{
	time_t exec_time = EXECUTE_NOW;
	int enqueue_task_success = SUCCESS;
	int when_to_do_task = EXECUTE_NOW;
	int is_task_success = SUCCESS;
	
	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	assert(YES_EMPTY != SchedulerIsEmpty(scheduler));
	
	while(ACTIVE == SchedulerIsActive(scheduler) && 
			YES_EMPTY != SchedulerIsEmpty(scheduler) &&
			FAIL != enqueue_task_success)
	{
		SetCurrentTaskIMP(scheduler, (task_t *)PQDeQueue(scheduler->pq));
		exec_time = TaskGetExecutionTime(GetCurrentTaskIMP(scheduler));
		when_to_do_task = SchedulerTimeDiffIMP(time(NOT_ASSIGNED), exec_time);
		while(EXECUTE_NOW < when_to_do_task)
		{
			when_to_do_task = sleep((unsigned int)when_to_do_task);
		}
		is_task_success = TaskExecute(GetCurrentTaskIMP(scheduler));
		if(NOT_ASSIGNED != GetCurrentTaskIMP(scheduler))
		{
			if(FAIL == is_task_success)
			{
				TaskDestroy(GetCurrentTaskIMP(scheduler));
			}
			else
			{
				exec_time += 
					(time_t)TaskGetInterval(GetCurrentTaskIMP(scheduler));
				TaskSetExecutionTime(GetCurrentTaskIMP(scheduler), exec_time);
				enqueue_task_success = PQEnQueue(scheduler->pq, 
									(const void *)GetCurrentTaskIMP(scheduler));
			}
		}
	}
	SetCurrentTaskIMP(scheduler, NOT_ASSIGNED);
	
	return (ACTIVE == SchedulerIsActive(scheduler));
}

void SchedulerClear(scheduler_t *scheduler)
{
	task_t *task_to_destroy = NOT_ASSIGNED;
	
	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	
	while(YES_EMPTY != SchedulerIsEmpty(scheduler))
	{
		task_to_destroy = (task_t *)PQDeQueue(scheduler->pq);
		TaskDestroy(task_to_destroy);
		task_to_destroy = NOT_ASSIGNED;
	}
	if(NULL != GetCurrentTaskIMP(scheduler))
	{
		TaskDestroy(GetCurrentTaskIMP(scheduler));
		SetCurrentTaskIMP(scheduler, NOT_ASSIGNED);
	}
}

ilrd_uid_t SchedulerInsertTask(scheduler_t *scheduler,
								int (*op_func)(void *func_params), 
								void *func_params, 
								size_t interval)
{
	task_t *new_task = NOT_ASSIGNED;
	int is_success = SUCCESS;

	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	assert(NOT_ASSIGNED != op_func);
	assert(0 != interval);
	
	new_task = TaskCreate(op_func, func_params, interval);
	if(NOT_ASSIGNED == new_task)
	{
		return BadUIDCreate();
	}
	TaskSetExecutionTime(new_task, time(NOT_ASSIGNED) + interval);
	is_success = PQEnQueue(scheduler->pq, (const void *)new_task);
	if(SUCCESS != is_success)
	{
		TaskDestroy(new_task);
		return BadUIDCreate();
	}
	
	return TaskGetUid(new_task);
}

int SchedulerRemoveTask(scheduler_t *scheduler, ilrd_uid_t uid)
{
	task_t *task_to_be_removed = NOT_ASSIGNED;
	ilrd_uid_t task_uid = BadUIDCreate();

	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	
	task_to_be_removed = (task_t *)PQErase(scheduler->pq, 
											SchedulerIsMAtchUIDIMP, 
											(const void *)&uid);
	
	if(NOT_ASSIGNED == task_to_be_removed && 
		NOT_ASSIGNED != GetCurrentTaskIMP(scheduler))
	{
		task_uid = TaskGetUid(GetCurrentTaskIMP(scheduler));
		if(UID_IS_EQUAL == UIDISEqual(&task_uid, &uid))
		{
			task_to_be_removed = GetCurrentTaskIMP(scheduler);
			SetCurrentTaskIMP(scheduler, NOT_ASSIGNED);
		}
	}
	if(NOT_ASSIGNED == task_to_be_removed)
	{
		return FAIL;
	}
	TaskDestroy(task_to_be_removed);
	
	return SUCCESS;
}

size_t SchedulerSize(scheduler_t *scheduler)
{
	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	
	return PQSize(scheduler->pq);
}

int SchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(NOT_ASSIGNED != scheduler);
	assert(NOT_ASSIGNED != scheduler->pq);
	
	return PQueueIsEmpty(scheduler->pq);
}

void SchedulerStop(scheduler_t *scheduler)
{
	assert(NOT_ASSIGNED != scheduler);
	
	atomic_store(&scheduler->is_active, NOT_ACTIVE);
}

int SchedulerIsActive(scheduler_t *scheduler)
{
	assert(NOT_ASSIGNED != scheduler);
	
	return (ACTIVE == atomic_load(&scheduler->is_active));
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
static int SchedulerTimeDiffIMP(time_t time1, time_t time2)
{
	return (int)(time2 - time1);
}

static int SchedulerCMPIMP(const void *data1, const void *data2, void *param)
{
	task_t *task1 = (task_t *)data1;
	task_t *task2 = (task_t *)data2;
	
	assert(NOT_ASSIGNED != data1);
	assert(NOT_ASSIGNED != data2);
	
	(void)param;
	
	return SchedulerTimeDiffIMP(TaskGetExecutionTime(task1), 
							TaskGetExecutionTime(task2));
}

static int SchedulerIsMAtchUIDIMP(const void *data1, const void *data2)
{	
	ilrd_uid_t task_uid = BadUIDCreate();
	ilrd_uid_t *uid = (ilrd_uid_t *)data2;

	assert(NOT_ASSIGNED != data1);
	assert(NOT_ASSIGNED != data2);
	
	task_uid = TaskGetUid((task_t *)data1);
	
	return 	UIDISEqual(&task_uid, uid);
}

static task_t *GetCurrentTaskIMP(scheduler_t *scheduler)
{
	assert(NOT_ASSIGNED != scheduler);
	
	return scheduler->current_task;
}

static void SetCurrentTaskIMP(scheduler_t *scheduler, task_t *task)
{
	assert(NOT_ASSIGNED != scheduler);
	
	scheduler->current_task = task;
}