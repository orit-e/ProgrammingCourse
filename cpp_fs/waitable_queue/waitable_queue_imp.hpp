/*******************************************************************************
 * 																			   *
 * 					File name:			waitable_queue_imp.hpp		  		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_WAITABLE_QUEUE_IMP_HPP
#define ILRD_RD9315_WAITABLE_QUEUE_IMP_HPP

#include <boost/thread/mutex.hpp>               // mutex, unique_lock
#include <boost/thread/condition_variable.hpp>  // condition_variable, 
                                                // notify_one(), wait_until()
#include <boost/chrono/duration.hpp>            // seconds,
                                                // system_clock::time_point,
                                                // system_clock::now()

#include "waitable_queue.hpp"

namespace ilrd
{

namespace details
{
template<typename QUEUE>
class IsNotEmpty
{
public:
    explicit IsNotEmpty(const QUEUE& theQueue_);
    bool operator()();
private:
    const QUEUE& m_queue;
};

template<typename QUEUE>
IsNotEmpty<QUEUE>::IsNotEmpty(const QUEUE& theQueue_)
: m_queue(theQueue_)
{
    // Empty on pupose.
}

template<typename QUEUE>
bool IsNotEmpty<QUEUE>::operator()()
{
    return !m_queue.IsEmpty();
}

} // namespace details

template <typename CONTAINER>
void WaitableQueue<CONTAINER>::Push(const typename CONTAINER::value_type& data)
{
    boost::unique_lock<WQMutex_t> lock(m_mutex);
    m_waitableQueue.push(data);
    m_condV.notify_one();
}

template <typename CONTAINER>
void WaitableQueue<CONTAINER>::Pop(typename CONTAINER::value_type& result)
{
    Pop(result, static_cast<boost::chrono::hours>(500));
}

template <typename CONTAINER>
bool WaitableQueue<CONTAINER>::Pop(typename CONTAINER::value_type& result, 
                                    boost::chrono::seconds timeout)
{
    using namespace boost::chrono;

    boost::unique_lock<WQMutex_t> lock(m_mutex);

    system_clock::time_point timedWait(system_clock::now() + timeout);
    details::IsNotEmpty< WaitableQueue<CONTAINER> > isNotEmpty(*this);

    bool status = m_condV.wait_until(lock, timedWait, isNotEmpty);
    if(true == status)
    {
        result = m_waitableQueue.front();
        m_waitableQueue.pop();    
    }

    return status;
}

template <typename CONTAINER>
bool WaitableQueue<CONTAINER>::IsEmpty() const
{
    return m_waitableQueue.empty();
}

} // namespace ilrd

#endif // ILRD_RD9315_WAITABLE_QUEUE_IMP_HPP