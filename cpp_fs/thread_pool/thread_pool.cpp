/*******************************************************************************
 * 																			   *
 * 					File name:			thread_pool.cpp		         		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy A								   *
 * 																			   *
 * ****************************************************************************/
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <iterator>
#include <boost/typeof/typeof.hpp>

#include "../waitable_queue/waitable_queue.hpp"
#include "../priority_queue_wrapper/priority_queue_wrapper.hpp"
#include "thread_pool.hpp"

namespace ilrd
{

namespace details
{

/******************************************************************************/
class PriorityTask : public ThreadPool::Task
{
public:
    explicit PriorityTask(ThreadPool::SPTask_t task_, 
                            ThreadPool::TaskPriority priority_);
    virtual void Run();
    ThreadPool::TaskPriority GetPriority();
private:
    ThreadPool::SPTask_t m_task;
    ThreadPool::TaskPriority m_priority;
};

PriorityTask::PriorityTask(ThreadPool::SPTask_t task_, 
                            ThreadPool::TaskPriority priority_)
: m_task(task_)
, m_priority(priority_)
{
    // Empty on purpose
}

void PriorityTask::Run()
{
    m_task->Run();
}

ThreadPool::TaskPriority PriorityTask::GetPriority()
{
    return m_priority;
}

/******************************************************************************/
void DoNothing()
{
    return;
}

/******************************************************************************/
class BadApple : public std::exception
{
public:
    const char *what() const noexcept;
};

const char *BadApple::what() const noexcept
{
    return "what what!";
}

void BadAppleFunc()
{
    throw BadApple();
}

} // namespace details

/******************************************************************************/
ThreadPool::ThreadPool(size_t numOfThreads_)
: m_numOfThreads(0)
, m_tasks()
, m_mutex()
, m_isRunning(false)
, m_checkIsPaused()
, m_threads()
{
    SetNumOfThreads(numOfThreads_);
}

ThreadPool::~ThreadPool() noexcept
{
    SetNumOfThreads(0);
    Pause();
    m_checkIsPaused.notify_all();
    typename Threads_t::reverse_iterator iter = m_threads.rbegin();
    size_t size = m_threads.size();
    while(0 < size--)
    {
        iter->get()->join();
        ++iter;
        m_threads.pop_back();
    }
}

void ThreadPool::Add(SPTask_t newTask_, const TaskPriority priority_)
{
    boost::unique_lock<TPMutex_t> lock(m_mutex);
    SetTask(newTask_, priority_);
}

void ThreadPool::Run()
{
    boost::unique_lock<TPMutex_t> lock(m_mutex);
    m_isRunning = true;
    m_checkIsPaused.notify_all();
}

void ThreadPool::Pause()
{
    boost::unique_lock<TPMutex_t> lock(m_mutex);
    m_isRunning = false;
    static SPTask_t doNothing = InitDoNothingTask();
    for(size_t i = 0; m_numOfThreads > i; ++i)
    {
        SetTask(doNothing, static_cast<TaskPriority>(3));
    }
}

void ThreadPool::Resume()
{
    boost::unique_lock<TPMutex_t> lock(m_mutex);
    m_isRunning = true;
    m_checkIsPaused.notify_all();
}

void ThreadPool::SetNumOfThreads(size_t size_)
{
    boost::unique_lock<TPMutex_t> lock(m_mutex);
    if(m_numOfThreads < size_)
    {
        AddThreads(size_ - m_numOfThreads);
    }
    else
    {
        SetBadAppleTasks(m_numOfThreads - size_);
    }
    m_numOfThreads = size_;
}

bool ThreadPool::IsPaused() const
{
    return m_isRunning;
}

void ThreadPool::ThreadExecution()
{
    SPTask_t taskToRun;
    while(true == CheckIfPaused())
    {
        bool status = m_tasks.Pop(taskToRun, 
                                    static_cast<boost::chrono::seconds>(1));
        if(true == status)
        {
            try
            {
                taskToRun->Run();
            }
            catch(details::BadApple& m_)
            {
                return;
            }
        }
    }
}

bool ThreadPool::CheckIfPaused()
{
    boost::unique_lock<TPMutex_t> lock(m_mutex);
    BOOST_AUTO(checker,boost::bind(&ThreadPool::IsPaused, this));

    return m_checkIsPaused.wait_until(lock, boost::chrono::hours(1) + 
                                boost::chrono::system_clock::now(), checker);

}

bool ThreadPool::MyCompare::operator()(ThreadPool::SPTask_t& lhs_, 
                                        ThreadPool::SPTask_t& rhs_)
{
        return (
            static_cast< details::PriorityTask *>(lhs_.get())->GetPriority() < 
            static_cast< details::PriorityTask *>(rhs_.get())->GetPriority());
}

ThreadPool::SPTask_t ThreadPool::InitDoNothingTask() // Higher Priority
{
    FunctionTask *DoNothingTask = new FunctionTask(&details::DoNothing);
    SPTask_t sPDoNothingTask(DoNothingTask);

    return sPDoNothingTask;
}

ThreadPool::SPTask_t ThreadPool::InitBadAppleTask() // Higher Priority
{
    FunctionTask *badAppleTask = 
                new FunctionTask(&details::BadAppleFunc);
    SPTask_t sPBadAppleTask(badAppleTask);

    return sPBadAppleTask;
}

void ThreadPool::AddThreads(size_t numOfNewThreads_)
{
    for(size_t i = 0; numOfNewThreads_ > i; ++i)
    {
        TPThread_t threadPtr(new Thread_ty(&ThreadPool::ThreadExecution, this));
        m_threads.push_back(threadPtr);
    }
}

void ThreadPool::SetTask(SPTask_t newTask_, const TaskPriority priority_)
{
    Task *newTask = new details::PriorityTask(newTask_, priority_);
    SPTask_t newSPTask(newTask);

    m_tasks.Push(newSPTask);
}

void ThreadPool::SetBadAppleTasks(size_t num_)
{
    SPTask_t badApple(InitBadAppleTask());
    for(size_t i = 0; num_ > i; ++i)
    {
        SetTask(badApple, static_cast<TaskPriority>(3));
    }
}

/******************************************************************************/
ThreadPool::Task::~Task()
{
    
}

/******************************************************************************/
ThreadPool::FunctionTask::FunctionTask(TPFunc_t func_)
: m_func(func_)
{
    
}

ThreadPool::FunctionTask::~FunctionTask()
{
    
}

void ThreadPool::FunctionTask::Run()
{
    m_func();
}

} // namespace ilrd