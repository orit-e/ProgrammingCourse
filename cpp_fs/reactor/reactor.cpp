/*******************************************************************************
 * 																			   *
 * 					File name:			reactor.cpp		         		       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Dvir								   *
 * 																			   *
 * ****************************************************************************/
#include <map>                              // insert(), find(), erase(), 
                                            // begin(), end()
#include <vector>                           // begin(), end(), erase()
#include <iterator>                         // map_t::iterator, 
                                            // VecFDs_t::iterator,
                                            // std::back_inserter
#include <exception>                        // class exception (inheritance)
#include <boost/foreach.hpp>                // BOOST_FOREACH()
#include <boost/thread.hpp>                 // boost::thread, detach()
#include <boost/range/adaptor/map.hpp>      // boost::adaptors::map_keys
#include <boost/range/algorithm/copy.hpp>   // boost::copy()
#include <errno.h>                          // errno
#include <cstring>                          // strerror()
#include <sys/select.h>                     // select(), FD_SET(), ISSET()

#include "reactor.hpp"

namespace ilrd
{
// Reactor class:
Reactor::Reactor()
: m_map()
, m_watcher()
, m_isRunning(false)
{
    // Empty on purpose.
}

void Reactor::Run()
{
    if(false == m_isRunning)
    {
        m_isRunning = true;
        Runner();
    }
}

void Reactor::Stop()
{
    if(true == m_isRunning)
    {
        m_isRunning = false;
    }
}

void Reactor::Add(Fd_t fd_, ModeType_t mode_, CallbackFunc_t reactFunc_)
{
    if(0 > fd_)
    {
        return;
    }

    std::pair<map_t::iterator, bool> iter = 
                    m_map.insert(mapPair_t(keyPair_t(fd_, mode_), reactFunc_));
    if(false == iter.second)
    {
        iter.first->second = reactFunc_;
    }
}

void Reactor::Remove(Fd_t fd_, ModeType_t mode_)
{
    map_t::iterator iter = m_map.find(keyPair_t(fd_, mode_));
    if(m_map.end() != iter)
    {
        m_map.erase(iter);
    }
}

void Reactor::Runner()
{
    timeval *tv = reinterpret_cast<timeval *>(operator new(sizeof(timeval)));
    tv->tv_sec = 1;
    tv->tv_usec = 0;
    while(true == m_isRunning)
    {
        Watcher::VecFDs_t fds;
        boost::copy(m_map | boost::adaptors::map_keys, 
                    std::back_inserter(fds));
        
        
        Watcher::VecFDs_t activeFds = m_watcher.Monitor(fds, tv);

        if(true == m_isRunning)
        {
            BOOST_FOREACH(const keyPair_t& key, activeFds)
            {
                typename map_t::iterator found = m_map.find(key);
                if(m_map.end() != found)
                {
                    found->second();
                }
            }
        }
    }
    operator delete(tv);
}

bool Reactor::IsStopped() const noexcept
{
    return !m_isRunning;
}

const char *Reactor::ReactorException::what() const noexcept
{
    return strerror(errno);
}

// Watcher class:
Watcher::VecFDs_t Watcher::Monitor(VecFDs_t fds_)
{
    return Watcher::Monitor(fds_, NULL);
}

Watcher::VecFDs_t Watcher::Monitor(VecFDs_t fds_, struct timeval *timeout)
{
    fd_set fdsSet[3];
    FD_ZERO(&fdsSet[REACT_READ]);
    FD_ZERO(&fdsSet[REACT_WRITE]);
    FD_ZERO(&fdsSet[REACT_EXCEPT]);
    Fd_t maxFd = 0;
    
    for(typename VecFDs_t::iterator iter = fds_.begin(); 
        fds_.end() != iter; ++iter)
    {
        if(maxFd < iter->first)
        {
            maxFd = iter->first;
        }
        FD_SET(iter->first, &fdsSet[iter->second]);
    }

    int activeFdsNum = select(maxFd + 1, 
                            &fdsSet[REACT_READ], 
                            &fdsSet[REACT_WRITE], 
                            &fdsSet[REACT_EXCEPT], 
                            timeout);
    if(-1 == activeFdsNum--)
    {
        throw(Reactor::ReactorException());
    }
    VecFDs_t activeFd;
    for(VecFDs_t::iterator iter = fds_.begin(); 
        fds_.end() != iter && 0 < activeFdsNum; ++iter)
    {
        if(0 != FD_ISSET(iter->first, &fdsSet[iter->second]))
        {
            activeFd.push_back(*iter);
            --activeFdsNum;
        }
    }

    return activeFd;
}

} // namespace ilrd