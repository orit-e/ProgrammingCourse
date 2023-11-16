/*******************************************************************************
 * 																			   *
 * 					File name:			test_thread_pool.cpp	     		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy A								   *
 * 																			   *
 * ****************************************************************************/
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <string>

#include "thread_pool.hpp"

using namespace ilrd;

enum PRINT_COLOR
{
    RESET_STYLE = 0,
    RED_COLOR = 31,
    GREEN_COLOR,
    YELLOW_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR
};

/******************************************************************************/
class MyTask : public ThreadPool::Task
{
public:
    MyTask(bool& checkMark_)
    : m_checkMark(checkMark_)
    {
        // Empty on purpose.
    }
    virtual ~MyTask()
    {
        // Empty on purpose.
    }
    virtual void Run()
    {
        m_checkMark ^= true;
    }
private:
    bool& m_checkMark;
};

/******************************************************************************/
ThreadPool::TaskPriority g_priorityArray[15];

class MyPriorityCheckerTask : public ThreadPool::Task
{
public:
    MyPriorityCheckerTask(ThreadPool::TaskPriority thePriority_)
    : m_thePriority(thePriority_)
    {
        // Empty on purpose.
    }
    virtual ~MyPriorityCheckerTask()
    {
        // Empty on purpose.
    }
    virtual void Run()
    {
        static size_t index = 0;
        g_priorityArray[index] = m_thePriority;
        ++index;
    }
private:
    ThreadPool::TaskPriority m_thePriority;
};

/******************************************************************************/
void TestTP1Thread1Task();
void TestTPManyThreadsManyTasks();
void TestTPPauseResume();
void TestTPFunctionTask();
void TheFunction();
void TestPriorityCMP();
void TestSetNumOfThreadsAndFutureTask();
size_t GetThreadId();
void PrintTestResult(int result);

bool g_checkMark = false;

int main()
{
    TestTP1Thread1Task();
    TestTPManyThreadsManyTasks();
    TestTPPauseResume();
    TestTPFunctionTask();
    TestPriorityCMP();
    TestSetNumOfThreadsAndFutureTask();

    return 0;
}

void TestTP1Thread1Task()
{
    std::cout << "TestTP1Thread1Task: ";

    bool checkMark = false;
    MyTask *myTask = new MyTask(checkMark);
    ThreadPool::SPTask_t sPMyTask(myTask);
    ThreadPool myTP(1);

    myTP.Add(sPMyTask);
    myTP.Run();
    sleep(1);
    
    int result = (true != checkMark);

    checkMark = false;
    myTP.Add(sPMyTask);
    sleep(1);
    result += (true != checkMark);

    PrintTestResult(result);
}

void TestTPManyThreadsManyTasks()
{
    std::cout << "TestTPManyThreadsManyTasks: ";

    bool checkMark1 = false;
    bool checkMark2 = false;
    bool checkMark3 = false;
    bool checkMark4 = false;
    MyTask *myTask1 = new MyTask(checkMark1);
    MyTask *myTask2 = new MyTask(checkMark2);
    MyTask *myTask3 = new MyTask(checkMark3);
    MyTask *myTask4 = new MyTask(checkMark4);
    ThreadPool::SPTask_t sPMyTask1(myTask1);
    ThreadPool::SPTask_t sPMyTask2(myTask2);
    ThreadPool::SPTask_t sPMyTask3(myTask3);
    ThreadPool::SPTask_t sPMyTask4(myTask4);

    ThreadPool myTP(3);

    myTP.Add(sPMyTask1, ThreadPool::NORMAL);
    myTP.Add(sPMyTask2, ThreadPool::NORMAL);
    myTP.Add(sPMyTask3, ThreadPool::NORMAL);
    myTP.Add(sPMyTask4, ThreadPool::NORMAL);
    myTP.Run();
    sleep(1);
    
    int result = (true != checkMark1);
    result += (true != checkMark2);
    result += (true != checkMark3);
    result += (true != checkMark4);

    myTP.Add(sPMyTask1, ThreadPool::NORMAL);
    myTP.Add(sPMyTask1, ThreadPool::NORMAL);
    sleep(1);
    result += (true != checkMark1);

    PrintTestResult(result);
}

void TestTPPauseResume()
{
    std::cout << "TestTPPauseResume: ";

    bool checkMark1 = false;
    bool checkMark2 = false;
    MyTask *myTask1 = new MyTask(checkMark1);
    MyTask *myTask2 = new MyTask(checkMark2);
    ThreadPool::SPTask_t sPMyTask1(myTask1);
    ThreadPool::SPTask_t sPMyTask2(myTask2);

    ThreadPool myTP(3);

    myTP.Add(sPMyTask1, ThreadPool::NORMAL);
    myTP.Add(sPMyTask2, ThreadPool::NORMAL);
    myTP.Add(sPMyTask2, ThreadPool::NORMAL);
    myTP.Run();
    sleep(1);
    
    int result = (true != checkMark1);
    result += (false != checkMark2);

    myTP.Pause();
    myTP.Add(sPMyTask1, ThreadPool::NORMAL);
    sleep(1);
    result += (true != checkMark1);
    myTP.Resume();
    sleep(1);
    result += (false != checkMark1);

    PrintTestResult(result);
}

void TestTPFunctionTask()
{
    std::cout << "TestTPFunctionTask: ";

    ThreadPool::FunctionTask *myFuncTask = 
                                    new ThreadPool::FunctionTask(&TheFunction);
    ThreadPool::SPTask_t sPMyFunc(myFuncTask);
    ThreadPool myTP(1);

    myTP.Add(sPMyFunc, ThreadPool::NORMAL);
    myTP.Run();
    sleep(1);
    
    int result = (true != g_checkMark);

    myTP.Add(sPMyFunc, ThreadPool::NORMAL);
    sleep(1);
    result += (false != g_checkMark);

    PrintTestResult(result);
}

void TheFunction()
{
    g_checkMark ^= true;
}

void TestPriorityCMP()
{
    std::cout << "TestPriorityCMP: ";

    MyPriorityCheckerTask *myTask1 = 
                        new MyPriorityCheckerTask(ThreadPool::LOW);
    MyPriorityCheckerTask *myTask2 = 
                        new MyPriorityCheckerTask(ThreadPool::NORMAL);
    MyPriorityCheckerTask *myTask3 = 
                        new MyPriorityCheckerTask(ThreadPool::HIGH);
    MyPriorityCheckerTask *myTask4 = 
                        new MyPriorityCheckerTask(ThreadPool::HIGH);
    MyPriorityCheckerTask *myTask5 = 
                        new MyPriorityCheckerTask(ThreadPool::NORMAL);
    MyPriorityCheckerTask *myTask6 = 
                        new MyPriorityCheckerTask(ThreadPool::LOW);
    MyPriorityCheckerTask *myTask7 = 
                        new MyPriorityCheckerTask(ThreadPool::HIGH);
    MyPriorityCheckerTask *myTask8 = 
                        new MyPriorityCheckerTask(ThreadPool::LOW);
    MyPriorityCheckerTask *myTask9 = 
                        new MyPriorityCheckerTask(ThreadPool::NORMAL);
    MyPriorityCheckerTask *myTask10 = 
                        new MyPriorityCheckerTask(ThreadPool::HIGH);
    MyPriorityCheckerTask *myTask11 = 
                        new MyPriorityCheckerTask(ThreadPool::NORMAL);
    MyPriorityCheckerTask *myTask12 = 
                        new MyPriorityCheckerTask(ThreadPool::LOW);
    MyPriorityCheckerTask *myTask13 = 
                        new MyPriorityCheckerTask(ThreadPool::NORMAL);
    MyPriorityCheckerTask *myTask14 = 
                        new MyPriorityCheckerTask(ThreadPool::LOW);
    MyPriorityCheckerTask *myTask15 = 
                        new MyPriorityCheckerTask(ThreadPool::HIGH);
    
    ThreadPool::SPTask_t sPMyTask1(myTask1);
    ThreadPool::SPTask_t sPMyTask2(myTask2);
    ThreadPool::SPTask_t sPMyTask3(myTask3);
    ThreadPool::SPTask_t sPMyTask4(myTask4);
    ThreadPool::SPTask_t sPMyTask5(myTask5);
    ThreadPool::SPTask_t sPMyTask6(myTask6);
    ThreadPool::SPTask_t sPMyTask7(myTask7);
    ThreadPool::SPTask_t sPMyTask8(myTask8);
    ThreadPool::SPTask_t sPMyTask9(myTask9);
    ThreadPool::SPTask_t sPMyTask10(myTask10);
    ThreadPool::SPTask_t sPMyTask11(myTask11);
    ThreadPool::SPTask_t sPMyTask12(myTask12);
    ThreadPool::SPTask_t sPMyTask13(myTask13);
    ThreadPool::SPTask_t sPMyTask14(myTask14);
    ThreadPool::SPTask_t sPMyTask15(myTask15);

    ThreadPool myTP(1);

    myTP.Add(sPMyTask1, ThreadPool::LOW);
    myTP.Add(sPMyTask2, ThreadPool::NORMAL);
    myTP.Add(sPMyTask3, ThreadPool::HIGH);
    myTP.Add(sPMyTask4, ThreadPool::HIGH);
    myTP.Add(sPMyTask5, ThreadPool::NORMAL);
    myTP.Add(sPMyTask6, ThreadPool::LOW);
    myTP.Add(sPMyTask7, ThreadPool::HIGH);
    myTP.Add(sPMyTask8, ThreadPool::LOW);
    myTP.Add(sPMyTask9, ThreadPool::NORMAL);
    myTP.Add(sPMyTask10, ThreadPool::HIGH);
    myTP.Add(sPMyTask11, ThreadPool::NORMAL);
    myTP.Add(sPMyTask12, ThreadPool::LOW);
    myTP.Add(sPMyTask13, ThreadPool::NORMAL);
    myTP.Add(sPMyTask14, ThreadPool::LOW);
    myTP.Add(sPMyTask15, ThreadPool::HIGH);
    
    myTP.Run();
    sleep(1);

    int result = 0;
    size_t i = 0;
    for(; 5 > i; ++i)
    {
        result += (ThreadPool::HIGH != g_priorityArray[i]);
    }
    for(; 10 > i; ++i)
    {
        result += (ThreadPool::NORMAL != g_priorityArray[i]);
    }
    for(; 15 > i; ++i)
    {
        result += (ThreadPool::LOW != g_priorityArray[i]);
    }

    PrintTestResult(result);
}

void TestSetNumOfThreadsAndFutureTask()
{
    std::cout << "TestSetNumOfThreadsAndFutureTask: ";

    ThreadPool::FutureTask<size_t> *myFuncTask1 = 
                    new ThreadPool::FutureTask<size_t>(&GetThreadId);
    ThreadPool::FutureTask<size_t> *myFuncTask2 = 
                    new ThreadPool::FutureTask<size_t>(&GetThreadId);
    ThreadPool::FutureTask<size_t> *myFuncTask3 = 
                    new ThreadPool::FutureTask<size_t>(&GetThreadId);
        
    ThreadPool::SPTask_t sPMyFunc1(myFuncTask1);
    ThreadPool::SPTask_t sPMyFunc2(myFuncTask2);
    ThreadPool::SPTask_t sPMyFunc3(myFuncTask3);

    ThreadPool myTP(1);

    myTP.Add(sPMyFunc1, ThreadPool::NORMAL);
    myTP.Run();
    sleep(1);

    boost::thread::id defaultRet;
    size_t firstId = myFuncTask1->GetReturnVal();
    int result = (hash_value(defaultRet) == firstId);

    myTP.SetNumOfThreads(0);
    sleep(1);
    myTP.SetNumOfThreads(3);
    myTP.Add(sPMyFunc1, ThreadPool::NORMAL);
    myTP.Add(sPMyFunc2, ThreadPool::NORMAL);
    myTP.Add(sPMyFunc3, ThreadPool::NORMAL);

    size_t ids[3] = 
    {
        myFuncTask1->GetReturnVal(), 
        myFuncTask2->GetReturnVal(),
        myFuncTask3->GetReturnVal()
    };
    result += (hash_value(defaultRet) == ids[0]);
    result += (ids[0] == ids[2]);
    result += (ids[1] == ids[0]);
    result += (ids[1] == ids[2]);

    PrintTestResult(result);
}

size_t GetThreadId()
{
    sleep(1);
    return hash_value(boost::this_thread::get_id());
}

void PrintTestResult(int result)
{
    (0 == result) ? 
    std::cout << "\033[" << GREEN_COLOR << "m"
              << "success!" 
              << "\033[" << RESET_STYLE << "m"  << std::endl :
    std::cout << "\033[" << RED_COLOR << "m" 
              << "test failed with " << result << " errors." 
              << "\033[" << RESET_STYLE << "m" << std::endl;            
}