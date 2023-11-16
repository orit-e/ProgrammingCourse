
#ifndef ILRD_RD9315_DIR_MONITOR_HPP
#define ILRD_RD9315_DIR_MONITOR_HPP

#include <string>                       // std::string
#include <utility>                      // std::pair
#include <map>
#include <exception>                    // std::exception inheritance

#include <boost/core/noncopyable.hpp>   // boost::noncopyable
#include <boost/thread.hpp>             // boost::thread
#include <boost/thread/mutex.hpp>       // boost::mutex
#include <boost/function.hpp>           // boost::function

#include <sys/inotify.h>                // IN_CREATE, IN_MOVED_TO,
                                        // IN_MODIFY

#include "../dispatcher_callback/dispatcher_callback.hpp" 
                                    // class dispatcher, class simpleCallback
#include "../utils/utils.hpp"       // noexcept

namespace ilrd
{

class DirMonitor : private boost::noncopyable
{
public:
    enum EVENT_TYPE
    {
        DIR_MONITOR_SO_LIB_MODIFY = IN_MODIFY,
        DIR_MONITOR_SO_LIB_MOVED_FROM = IN_MOVED_FROM,
        DIR_MONITOR_SO_LIB_MOVED_TO = IN_MOVED_TO,
        DIR_MONITOR_SO_LIB_CREATE = IN_CREATE,
        DIR_MONITOR_SO_LIB_DELETE = IN_DELETE,
        DIR_MONITOR_SO_LIB_EVENT = (IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO |
                                    IN_CREATE | IN_DELETE)
    };
    typedef std::pair<std::string, EVENT_TYPE> data_t;

    explicit DirMonitor(const std::string& dirname_); // may trow bad_alloc, 
                                                      // FileSystemError
    ~DirMonitor() noexcept;
    
    void Subscribe(CallbackBase<data_t, DirMonitor> *cb_); // may throw 
                                                           // bad_alloc
    void UnSubscribe(CallbackBase<data_t, DirMonitor> *cb_); // may throw 
                                                             // bad_alloc

    class FileSystemError : public std::exception
    {
    public:
        const char *what() const noexcept;
    };

private:
    void Watch(); // may trow bad_alloc, FileSystemError
    
    struct fds_t
    {
        int s_wfd;
        int s_inotifyFd;
    };
    
    fds_t InitFds(); // may trow FileSystemError
    
    Dispatcher<data_t, DirMonitor> m_dispatcher;
    const std::string m_dirname;
    fds_t m_fds;
    bool m_isRunning;
    boost::mutex m_mutex;
    boost::thread m_watch;
};

class DLLLoader : private boost::noncopyable
{
public:
    explicit DLLLoader(DirMonitor& dirMonitor_); // may throw bad_alloc
    ~DLLLoader() noexcept; //???

    class DLException : public std::exception
    {
    public:
        const char *what() const noexcept;
    };
private:
    typedef std::pair<const std::string, void *> MapPair_t;
    typedef std::map<std::string, void *> SymHandlers_t;

    void Update(DirMonitor::data_t& d_); // may throw 
                                         // DirMonitor::FileSystemFail, 
                                         // DLException
    void UpdateDeath();
    void DLCloseEnvelop(MapPair_t& mapPair_);

    DirMonitor& m_dirMonitor;
    SimpleCallback<DirMonitor::data_t, DirMonitor> m_callback;
    SymHandlers_t m_symHandlers;
};

} // namespace ilrd

#endif // ILRD_RD9315_DIR_MONITOR_HPP
