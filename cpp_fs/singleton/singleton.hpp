#ifndef ILRD_RD9315_SINGLETON_HPP
#define ILRD_RD9315_SINGLETON_HPP

#include <exception>
#include <boost/core/noncopyable.hpp>   // boost::noncopyable
#include <boost/thread/mutex.hpp>       // boost::mutex
#include <boost/atomic.hpp>             // boost::atomic<T *>

#include "../utils/utils.hpp"

namespace ilrd
{

template <typename T>
class Singleton : private boost::noncopyable
{
public:
    static T& GetInstance();

    class SingletonException : public std::exception
    {
    public:
        const char *what() const noexcept;
    };
    
private:
    Singleton();
    
    static boost::atomic<T *> m_instance;
    static boost::mutex m_mutex;
    static const T *BAD_PTR; 
    
    static void InstanceDestroyer();
};

} // namespace ilrd

#include "singleton_imp.hpp"

#endif // ILRD_RD9315_SINGLETON_HPP