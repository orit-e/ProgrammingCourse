/********************************************
 *											*
 *		filename:			task.c			*
 *		written by:     	Orit			*
 *		reviewed by:    	Dor				*
 *		last edit:      	12.08.2020		*
 *											*
 ********************************************/
#include <stdlib.h>	/* malloc, free, size_t */
#include <string.h>	/* memset */
#include <assert.h>	/* assert */
#include "task.h"

#define ON 1
#define OFF 0

struct task
{
    ilrd_uid_t uid;
    op_func_t op_func;
    void *func_params;
    size_t interval;
    time_t exec_time;
    int exec_state;
};

task_t *TaskCreate(int (*op_func)(void *func_params), 
									void *func_params, 
									size_t interval)
{
	task_t *new_task = (task_t *)malloc(sizeof(task_t));
	
	assert(NULL != op_func);
	assert(0 != interval);
	
	if(NULL == new_task)
	{
		return NULL;
	}
	memset(new_task, 0, sizeof(task_t));
	new_task->uid = UIDCreate();
	new_task->op_func = op_func;
	new_task->func_params = func_params;
	new_task->interval = interval;
	new_task->exec_state = ON;
	
	return new_task;
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	
	memset(task, 0, sizeof(task_t));
	free(task);
	task = NULL;
}

int TaskExecute(task_t *task)
{
	assert(NULL != task);
	assert(NULL != task->op_func);
	
	return task->op_func(task->func_params);
}

void TaskSetExecutionTime(task_t *task, time_t new_execution_time)
{
	assert(NULL != task);
	
	task->exec_time = new_execution_time;
}

time_t TaskGetExecutionTime(const task_t *task)
{
	assert(NULL != task);
	
	return task->exec_time;
}

size_t TaskGetInterval(const task_t *task)
{
	assert(NULL != task);
	
	return task->interval;
}

void TaskShutDown(task_t *task)
{
	assert(NULL != task);
	
	task->exec_state = OFF;
}

void TaskTurnOn(task_t *task)
{
	assert(NULL != task);
	
	task->exec_state = ON;
}

int TaskIsTurnedOn(task_t *task)
{
	assert(NULL != task);
	
	return task->exec_state;
}

ilrd_uid_t TaskGetUid(task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

