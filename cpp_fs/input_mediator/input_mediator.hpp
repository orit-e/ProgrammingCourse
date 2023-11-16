#ifndef ILRD_RD9315_INPUT_MEDIATOR_HPP
#define ILRD_RD9315_INPUT_MEDIATOR_HPP

#include <iosfwd>
#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "../reactor/reactor.hpp"
#include "../thread_pool/thread_pool.hpp"
#include "../utils/utils.hpp"

namespace ilrd
{

struct Data
{
    std::string s_str;
    boost::shared_ptr<void> s_data;
};

class Proxy
{
public:
    typedef int Fd_t;
    virtual ~Proxy();
    virtual boost::shared_ptr<Data> ParseData(Fd_t fd_) = 0;
};

class Command
{
public:
    virtual ~Command() noexcept;
    virtual void Run() = 0;
};

class InputMediator
{
public:
    typedef int                                             Fd_t;
    typedef boost::shared_ptr<Command>                      cmd_t;
    typedef boost::function<cmd_t(boost::shared_ptr<Data>)> CreateFunc_t;
    typedef CreateFunc_t                                    Callable_t;
    struct IMPipeFds
    {
        Fd_t s_read;
        Fd_t s_write;
    };

    explicit InputMediator(CreateFunc_t creator_, ThreadPool *threadPool_);
    ~InputMediator(); //???

    void Subscribe(boost::shared_ptr<Proxy> proxy_, Fd_t readFd_);
    void Unsubscribe(Fd_t readFd_);
    
    void ReactorStart();
    void ReactorStop();

    class IMBadFdException : public std::exception
    {
    public:
        const char *what() const noexcept;
    };

private:
    typedef std::pair<Fd_t, boost::shared_ptr<Proxy> > ProxiesPair_t;
    typedef std::map<Fd_t, boost::shared_ptr<Proxy> > Proxies_t;

    enum ReactorActions
    {
        REACTOR_ADD = 1,
        REACTOR_REMOVE,
        REACTOR_STOP
    };

    void ReactorCallback(Fd_t fd_);


    Proxies_t m_proxies;
    Reactor m_reactor;
    boost::shared_ptr<boost::thread> m_reactorRun;
    CreateFunc_t m_creator;
    ThreadPool *m_threadPool;


    class RunTask : public ThreadPool::Task
    {
    public:
        RunTask(boost::shared_ptr<Data> data_, CreateFunc_t creator_);
        virtual ~RunTask();
        virtual void Run();

    private:
        boost::shared_ptr<Data> m_data;
        CreateFunc_t m_creator;
    };
};

} // namespace ilrd

#endif // ILRD_RD9315_INPUT_MEDIATOR_HPP