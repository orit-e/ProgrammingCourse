/*******************************************************************************
 * 																			   *
 * 					File name:			scope_lock.hpp				           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_SCOPE_LOCK_HPP
#define ILRD_RD9315_SCOPE_LOCK_HPP

#include <boost/core/noncopyable.hpp>

namespace ilrd
{

template<typename T> // T must have Lock() and unlock() as member functions
class ScopeLock : private boost::noncopyable
{
public:
    explicit ScopeLock(T& lock_);
    ~ScopeLock();

private:
    T& m_lock;
};

template<typename T>
ScopeLock<T>::ScopeLock(T& lock_)
: m_lock(lock_)
{
    m_lock.lock();
}

template<typename T>
ScopeLock<T>::~ScopeLock()
{
    m_lock.unlock();
}

} // namespace ilrd

#endif // ILRD_RD9315_SCOPE_LOCK_HPP