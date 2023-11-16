#include <map>
#include <iterator>
#include <algorithm>
#include <utility>

#include <sys/inotify.h>    // inotify_init(), inotify_add_watch(), 
                            // inotify_rm_watch(), IN_* masks
#include <cstring>          // strstr(), strerror()
#include <dlfcn.h>          // dlopen()
#include <errno.h>          // errno
#include <boost/bind.hpp>   // boost::bind()

#include "dir_monitor.hpp"
#include "../dispatcher_callback/dispatcher_callback.hpp"

namespace ilrd
{

const char *DirMonitor::FileSystemError::what() const noexcept
{
    return strerror(errno);
}

// DirMonitor definitions:
DirMonitor::DirMonitor(const std::string& dirname_)
: m_dispatcher()
, m_dirname(dirname_)
, m_fds(InitFds())
, m_isRunning(true)
, m_mutex()
, m_watch(&DirMonitor::Watch, this)
{
    // Empty on purpose.
}

DirMonitor::~DirMonitor() noexcept
{
    m_isRunning = false;
    inotify_rm_watch(m_fds.s_inotifyFd, m_fds.s_wfd);
    m_watch.join();
}

void DirMonitor::Subscribe(CallbackBase<data_t, DirMonitor> *cb_)
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    m_dispatcher.Subscribe(cb_);
}

void DirMonitor::UnSubscribe(CallbackBase<data_t, DirMonitor> *cb_)
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    m_dispatcher.UnSubscribe(cb_);
}

void DirMonitor::Watch()
{
    size_t sizeOfBuffer = sizeof(inotify_event) + NAME_MAX + 1;
    inotify_event *buffer = 
                    static_cast<inotify_event *>(operator new(sizeOfBuffer));
    std::string fileName;
    ssize_t readStatus = 0;

    while(true == m_isRunning)
    {
        readStatus = read(m_fds.s_inotifyFd, buffer, sizeOfBuffer);
        if(-1 == readStatus)
        {
            throw(FileSystemError());
        }
        if(0 != (DIR_MONITOR_SO_LIB_EVENT & buffer->mask) && 
            NULL != strstr(buffer->name, ".so"))
        {
            fileName = m_dirname + "/" + buffer->name;
            data_t data(fileName, static_cast<EVENT_TYPE>(buffer->mask));
            
            boost::unique_lock<boost::mutex> lock(m_mutex);
            m_dispatcher.Broadcast(data);
        }
    }

    operator delete(buffer);
}

DirMonitor::fds_t DirMonitor::InitFds()
{
    fds_t fds;
    fds.s_inotifyFd = inotify_init();
    if(-1 == fds.s_inotifyFd)
    {
        throw(FileSystemError());
    }
    
    fds.s_wfd = inotify_add_watch(fds.s_inotifyFd, m_dirname.c_str(), 
                                    DIR_MONITOR_SO_LIB_EVENT | 
                                    IN_EXCL_UNLINK | IN_ONLYDIR | 
                                    IN_MASK_CREATE | IN_IGNORED);
    if(-1 == fds.s_wfd)
    {
        throw(FileSystemError());
    }

    return fds;
}


// DLLLoader definitions:
DLLLoader::DLLLoader(DirMonitor& dirMonitor_)
: m_dirMonitor(dirMonitor_)
, m_callback(&m_dirMonitor, boost::bind(&DLLLoader::Update, this, _1), 
                boost::bind(&DLLLoader::UpdateDeath, this))
, m_symHandlers()
{
    // Empty on purpose.
}

DLLLoader::~DLLLoader() noexcept //???
{
    std::for_each(m_symHandlers.begin(), m_symHandlers.end(), 
                    boost::bind(&DLLLoader::DLCloseEnvelop, this, _1));
}

void DLLLoader::DLCloseEnvelop(MapPair_t& mapPair_)
{
    dlclose(mapPair_.second);
}

void DLLLoader::Update(DirMonitor::data_t& d_)
{
    if(0 != ((IN_MODIFY | IN_MOVED_FROM | IN_DELETE) & d_.second))
    {
        SymHandlers_t::iterator iter =  m_symHandlers.find(d_.first);
        if(m_symHandlers.end() != iter)
        {
            //dlclose(iter->second);
            m_symHandlers.erase(iter);
        }
    }

    if(0 != ((IN_MODIFY | IN_MOVED_TO | IN_CREATE) & d_.second))
    {
        void *newSymHandle = dlopen(d_.first.c_str(), RTLD_LAZY);
        if(NULL == newSymHandle)
        {
            throw(DirMonitor::FileSystemError());
        }

        std::pair<SymHandlers_t::iterator, bool> isInserted =
                        m_symHandlers.insert(MapPair_t(d_.first, newSymHandle));
        if(false == isInserted.second)
        {
            throw(DLException());
        }
    }
}

void DLLLoader::UpdateDeath()
{
    // Empty on purpose.
}

const char *DLLLoader::DLException::what() const noexcept
{
    return "Failed to insert symbol handler to symHandlers map";
}

} // namespace ilrd