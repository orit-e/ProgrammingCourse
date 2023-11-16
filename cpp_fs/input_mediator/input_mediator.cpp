#include <utility>                          // std::pair<>()
#include <map>                              // m_proxies, find(), insert(), 
                                            // erase()
#include <exception>                        // throw(), IMBadFdException::what()

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "../reactor/reactor.hpp"
#include "../thread_pool/thread_pool.hpp"
#include "input_mediator.hpp"

namespace ilrd
{

// Proxy base class:
Proxy::~Proxy()
{
    // Empty on purpose.
}

// Command base class:
Command::~Command() noexcept
{
    // Empty on purpose.
}

//******************************************************************************
// InputMediator:
InputMediator::InputMediator(CreateFunc_t creator_, ThreadPool *threadPool_)
: m_proxies()
, m_reactor()
, m_reactorRun()
, m_creator(creator_)
, m_threadPool(threadPool_)
{
    // Empty on purpose.
}

InputMediator::~InputMediator()
{
    ReactorStop();
    m_reactorRun->join();
}

void InputMediator::Subscribe(boost::shared_ptr<Proxy> proxy_, Fd_t readFd_)
{
    m_proxies.insert(std::pair<Fd_t, boost::shared_ptr<Proxy> >(readFd_, 
                                                                proxy_));
    m_reactor.Add(readFd_, Watcher::REACT_READ, 
                    boost::bind(&InputMediator::ReactorCallback, this, 
                                readFd_));
}

void InputMediator::Unsubscribe(Fd_t readFd_)
{
    m_reactor.Remove(readFd_, Watcher::REACT_READ);
    m_proxies.erase(readFd_);
}

void InputMediator::ReactorStart()
{
    m_reactorRun.reset(new boost::thread(&Reactor::Run, &m_reactor));
}

void InputMediator::ReactorStop()
{
    m_reactor.Stop();
}

void InputMediator::ReactorCallback(Fd_t fd_)
{
    Proxies_t::iterator iter = m_proxies.find(fd_);
    if(m_proxies.end() == iter)
    {
        throw(IMBadFdException());
    }
    boost::shared_ptr<Data> data = iter->second->ParseData(fd_);

    ThreadPool::SPTask_t runTask(new RunTask(data, m_creator));
    m_threadPool->Add(runTask);
}


//******************************************************************************
// RunTask:
InputMediator::RunTask::RunTask(boost::shared_ptr<Data> data_, 
                                CreateFunc_t creator_)
: m_data(data_)
, m_creator(creator_)
{
    // Empty on purpose?
}

InputMediator::RunTask::~RunTask()
{
    // Empty on purpose?
}

void InputMediator::RunTask::Run()
{
    boost::shared_ptr<Command> cmd = m_creator(m_data);
    cmd->Run();
}


const char *InputMediator::IMBadFdException::what() const noexcept
{
    return "File descriptor is not subscribed to this InputMediator object.";
}



} // namespace ilrd