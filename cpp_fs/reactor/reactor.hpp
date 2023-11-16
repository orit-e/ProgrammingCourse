/*******************************************************************************
 * 																			   *
 * 					File name:			reactor.hpp		         		       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Dvir								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_REACTOR_HPP
#define ILRD_RD9315_REACTOR_HPP

// Standard includes
#include <map>
#include <vector>
#include <exception>

// 3rd party includes
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

// Local includes
#include "../utils/utils.hpp"

namespace ilrd
{
class Watcher
{
public:
    enum mode_type
    {
        REACT_READ = 0,
        REACT_WRITE,
        REACT_EXCEPT
    };
    typedef int Fd_t;
    typedef std::pair<Fd_t, mode_type> vectorPair_t;
    typedef std::vector<vectorPair_t> VecFDs_t;

    // Compiler generated
    // explicit Watcher();
    // virtual ~Watcher() noexcept;
    // Watcher(const Watcher& other);
    // Watcher& operator=(const Watcher& other);

    static VecFDs_t Monitor(VecFDs_t fds_);

    static VecFDs_t Monitor(VecFDs_t fds_, struct timeval *timeout);
};

class Reactor : boost::noncopyable
{
public:
    typedef boost::function<void()> CallbackFunc_t;
    typedef int Fd_t;
    typedef Watcher::mode_type ModeType_t;

    explicit Reactor();

    // Compiler generated
    // virtual ~Reactor() noexcept;

    void Run(); // may throw
    void Stop();

    bool IsStopped() const noexcept; // for tests.

    void Add(Fd_t fd_, ModeType_t mode_, CallbackFunc_t reactFunc_);
    void Remove(Fd_t fd_, ModeType_t mode_);
    
    class ReactorException : public std::exception
    {
    public:
        const char *what() const noexcept;
    };

private:
    typedef std::pair<Fd_t, ModeType_t> keyPair_t;
    typedef std::pair<keyPair_t, CallbackFunc_t> mapPair_t;
    typedef std::map<keyPair_t, CallbackFunc_t> map_t;
    
    map_t m_map;
    Watcher m_watcher;
    bool m_isRunning;

    void Runner();

}; // Reactor class

} // namespace ilrd

#endif // ILRD_RD9315_REACTOR_HPP