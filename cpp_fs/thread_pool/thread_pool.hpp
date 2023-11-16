/*******************************************************************************
 * 																			   *
 * 					File name:			thread_pool.hpp		         		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy A								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_THREAD_POOL_HPP
#define ILRD_RD9315_THREAD_POOL_HPP

#include <iosfwd>
#include <vector>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include "../utils/utils.hpp"
#include "../waitable_queue/waitable_queue.hpp"
#include "../priority_queue_wrapper/priority_queue_wrapper.hpp"

namespace ilrd
{

class ThreadPool : private boost::noncopyable
{
public:
    class Task;

    typedef boost::function<void()>                 TPFunc_t;
    typedef boost::shared_ptr<Task>                 SPTask_t;
    
    enum TaskPriority
    {
        LOW = 0,
        NORMAL,
        HIGH
    };

    explicit ThreadPool(size_t numOfThreads_ = 
                        boost::thread::hardware_concurrency());
    ~ThreadPool() noexcept;

    void Add(SPTask_t newTask_, TaskPriority priority_ = NORMAL); 
    void Run();     // Create threads.
    void Pause();   // Stop all tasks without terminating threads.
    void Resume();  // Resume all paused threads.
    void SetNumOfThreads(size_t size_); // reducing the number of threads 
                                        // doesn't happen immediately and take
                                        // some time after the function returns.
    

    class Task : private boost::noncopyable
    {
    public:
        virtual ~Task();
        virtual void Run()=0;
    };
    
    class FunctionTask : public Task
    {
    public:
        FunctionTask(TPFunc_t func_);
        virtual ~FunctionTask();
        virtual void Run();
    private:
        TPFunc_t m_func;
    };

    template <typename OUTPUT>
    class FutureTask : public Task
    {
    public:
        typedef boost::function<OUTPUT()> FTFunc_t;
        typedef boost::interprocess::interprocess_semaphore FTSem_t;

        FutureTask(FTFunc_t func_);
        virtual ~FutureTask();
        virtual void Run();
        OUTPUT GetReturnVal();

    private:
        FTFunc_t m_func;
        OUTPUT m_returnVal;
        FTSem_t m_sem;
    };
    
    class MyCompare
    {
    public:
        bool operator()(ThreadPool::SPTask_t& lhs_, ThreadPool::SPTask_t& rhs_);
    };

private:
    typedef WaitableQueue< PQWrapper<SPTask_t, 
                            std::vector<SPTask_t>, 
                            MyCompare> >           WQTask_t;
    typedef boost::shared_ptr<WQTask_t>            TPTask_t;
    typedef boost::mutex                           TPMutex_t;
    typedef boost::condition_variable              TPCondV_t;
    typedef boost::thread                          Thread_ty;
    typedef boost::shared_ptr<Thread_ty>           TPThread_t;
    typedef std::vector<TPThread_t>                Threads_t;

    size_t m_numOfThreads;
    WQTask_t m_tasks;
    TPMutex_t m_mutex;
    bool m_isRunning;
    TPCondV_t m_checkIsPaused;
    Threads_t m_threads;

    void ThreadExecution();
    bool IsPaused() const;
    bool CheckIfPaused();
    SPTask_t InitDoNothingTask();
    SPTask_t InitBadAppleTask();
    void AddThreads(size_t numOfNewThreads_);
    void SetTask(SPTask_t newTask_, const TaskPriority priority_);
    void SetBadAppleTasks(size_t num_);

};

} // namespace ilrd

#include "future_task_imp.hpp"

#endif // ILRD_RD9315_THREAD_POOL_HPP