#include <stdio.h> /* printf */
#include "task.h"

#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"
#define BLUE_COLOR		"\x1b[0;34m"

enum EXIT_STATUS {FAILED, SUCCESS, DONE};

#define RUN_TEST(result, name) {\
			printf("test %s: ", name);\
			switch (result)\
			{\
				case SUCCESS:\
					printf(GREEN_COLOR);\
					printf("Success\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case FAILED:\
					printf(RED_COLOR);\
					printf("Fail\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case DONE:\
					printf(BLUE_COLOR);\
					printf("Done\n");\
					printf(DEFAULT_COLOR);\
			};\
		}

int TestTaskCreate();
int TestTaskExecute();
int TestTaskSetExecutionTime();
int TestTaskGetExecutionTime();
int TestTaskGetInterval();
int TestTaskShutDown();
int TestTaskTurnOn();
int TestTaskIsTurnedOn();
int TestTaskGetUid();
int TestTaskDestroy();
int Match(void *func_params);

struct task
{
    ilrd_uid_t uid;
    op_func_t op_func;
    void *func_params;
    size_t interval;
    time_t exec_time;
    int exec_state;
};


task_t *task1 = NULL;
task_t *task2 = NULL;

int a = 1;
int b = 2;

int Match(void *func_params)
{
	
	return (1 == *(int *)func_params);
}

int main(int argc, char *argv[])
{
	RUN_TEST(TestTaskCreate(),					"TaskCreate");
	RUN_TEST(TestTaskExecute(),					"TaskExecute");
	RUN_TEST(TestTaskSetExecutionTime(),		"TaskSetExecutionTime");
	RUN_TEST(TestTaskGetExecutionTime(),		"TaskGetExecutionTime");
	RUN_TEST(TestTaskGetInterval(),				"TaskGetInterval");
	RUN_TEST(TestTaskShutDown(),				"TaskShutDown");
	RUN_TEST(TestTaskTurnOn(),					"TaskTurnOn");
	RUN_TEST(TestTaskIsTurnedOn(),				"TaskIsTurnedOn");
	RUN_TEST(TestTaskGetUid(),					"TaskGetUid");
	RUN_TEST(TestTaskDestroy(),					"TaskDestroy");

	(void) argc;
	(void) argv;

    return 0;
}

int TestTaskCreate()
{
	int result = 0;
	
	task1 = TaskCreate(Match, &a, 5);
	result += (NULL != task1);
	result += (-1 < task1->uid.pid);
	result += (-1 < task1->uid.time_stamp);
	result += (0 < task1->uid.counter);
	result += (&Match == task1->op_func);
	result += (&a == task1->func_params);
	result += (5 == task1->interval);
	result += (1 == task1->exec_state);
	
	return (8 == result);
}

int TestTaskExecute()
{
	int result = 0;
	
	result += (1 == TaskExecute(task1));
	task2 = TaskCreate(Match, &b, 5);
	result += (0 == TaskExecute(task2));
	
	return (2 == result);
}

int TestTaskSetExecutionTime()
{
	TaskSetExecutionTime(task1, 582l);
	
	return (582l == task1->exec_time);
}

int TestTaskGetExecutionTime()
{
	return (582l == TaskGetExecutionTime(task1));
}

int TestTaskGetInterval()
{
	return (5 == TaskGetInterval(task1));
}

int TestTaskShutDown()
{
	TaskShutDown(task1);
	
	return (0 == task1->exec_state);
}

int TestTaskTurnOn()
{
	TaskTurnOn(task1);
	
	return (1 == task1->exec_state);
}

int TestTaskIsTurnedOn()
{
	int result = 0;
	
	result += (1 == TaskIsTurnedOn(task1));
	TaskShutDown(task1);
	result += (0 == TaskIsTurnedOn(task1));
	
	return (2 == result);
}

int TestTaskGetUid()
{	
	int result = 0;
	ilrd_uid_t temp_uid = TaskGetUid(task1);
	
	result += (task1->uid.pid == temp_uid.pid);
	result += (task1->uid.time_stamp == temp_uid.time_stamp);
	result += (task1->uid.counter == temp_uid.counter);
	
	return (3 == result);
}

int TestTaskDestroy()
{
	TaskDestroy(task1);
	TaskDestroy(task2);
	
	return DONE;
}
