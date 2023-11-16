/*******************************************************************************
 * 																			   *
 * 					File name:			waitable_queue.hpp		  		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_WAITABLE_QUEUE_HPP
#define ILRD_RD9315_WAITABLE_QUEUE_HPP

#include <boost/core/noncopyable.hpp>           // class noncopyable
#include <boost/thread/mutex.hpp>               // boost::mutex
#include <boost/thread/condition_variable.hpp>  // boost::condition_variable
#include <boost/chrono/duration.hpp>            // boost::chrono::seconds

#include "../utils/utils.hpp"  // noexcept 

namespace ilrd
{   


template <typename CONTAINER>
class WaitableQueue : private boost::noncopyable
{
public:
    typedef boost::mutex WQMutex_t;
    typedef boost::condition_variable WQCondV_t;

    void Push(const typename CONTAINER::value_type& data);
    void Pop(typename CONTAINER::value_type& result);
    bool Pop(typename CONTAINER::value_type& result, 
                boost::chrono::seconds timeout);
    bool IsEmpty() const;

private:
    WQMutex_t m_mutex;
    WQCondV_t m_condV;
    CONTAINER m_waitableQueue; 

};


} // namespace ilrd

#include "waitable_queue_imp.hpp"

#endif // ILRD_RD9315_WAITABLE_QUEUE_HPP