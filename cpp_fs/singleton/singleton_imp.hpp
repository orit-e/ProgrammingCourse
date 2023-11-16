#ifndef ILRD_RD9315_SINGLETON_IMP_HPP
#define ILRD_RD9315_SINGLETON_IMP_HPP

#include <exception>
#include <boost/memory_order.hpp>   // boost::memory_order_acquire, 
                                    // boost::memory_order_relaxed,
                                    // boost::memory_order_release
#include <boost/atomic/atomic.hpp>  // load(), store()


#ifndef NDEBUG
#include <iostream>
#include <fstream>
#endif

#include "singleton.hpp"

namespace ilrd
{
template <typename T>
boost::atomic<T *> Singleton<T>::m_instance(NULL);

template <typename T>
boost::mutex Singleton<T>::m_mutex;

template <typename T>
const T * Singleton<T>::BAD_PTR = 
                                reinterpret_cast<const T*>(0xDEADBEEFDEADBEEF);

template <typename T>
T& Singleton<T>::GetInstance()
{
    if(NULL == m_instance.load(boost::memory_order_acquire))
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        if(NULL == m_instance.load(boost::memory_order_acquire))
        {
            m_instance.store(new T(), boost::memory_order_release);

            int status = atexit(&InstanceDestroyer);
            if(0 != status)
            {
                InstanceDestroyer();
                throw(SingletonException());
            }

        }
    }

    return *m_instance;
}

template <typename T>
void Singleton<T>::InstanceDestroyer()
{
    delete m_instance;
    m_instance.store(const_cast<T*>(BAD_PTR), boost::memory_order_release);
#ifndef NDEBUG
    std::fstream logger("singelton_log.txt", std::fstream::out | 
                                            std::fstream::app);
    logger << m_instance.load() << std::endl;
#endif // NDEBUG
}

template <typename T>
const char *Singleton<T>::SingletonException::what() const noexcept
{
    return "atexit() failed, instance was not created.";
}

} // namespace ilrd


#endif // ILRD_RD9315_SINGLETON_IMP_HPP