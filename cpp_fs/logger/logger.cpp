/*******************************************************************************
 * 																			   *
 * 					File name:			logger.cpp		         		       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Yona								   *
 * 																			   *
 * ****************************************************************************/
#include <string>                           // c_str(), size()

#include <boost/smart_ptr/shared_ptr.hpp>   
#include <boost/bind.hpp>                   
#include <boost/function.hpp>               

#include <sys/types.h>                      // open()
#include <sys/stat.h>                       // open()
#include <fcntl.h>                          // open()
#include <unistd.h>                         // close(), write(), fsync()
#include <errno.h>                          // errno
#include <cstring>                          // strerror()


#include "logger.hpp"
#include "../thread_pool/thread_pool.hpp"

namespace ilrd
{

namespace details
{
class LogTask : public ThreadPool::Task
{
public:
    LogTask(Logger::fd_t logFd_, std::string msg_);
    virtual ~LogTask();
    virtual void Run();
private:
    std::string m_msg;
    Logger::fd_t m_logFd;
};

LogTask::LogTask(Logger::fd_t logFd_, std::string msg_)
: m_msg(msg_ + "\n")
, m_logFd(logFd_)
{
    // Empty on purpose.
}

LogTask::~LogTask()
{
    // Empty on purpose.
}

void LogTask::Run()
{
    int status = write(m_logFd, m_msg.c_str(), m_msg.size());
    if(-1 == status)
    {
        throw(Logger::FileSystemError());
    }
}
} // namespace details
//******************************************************************************
bool Logger::LastTask()
{
    int status = fsync(m_logFD);
    if(-1 == status)
    {
        throw(FileSystemError());
    }

    return (status != -1);
}

const char *Logger::FileSystemError::what() const noexcept
{
    return strerror(errno);
}

Logger::Logger()
: m_path(InitPath())
, m_logFD(InitLogFile())
, m_threadPool(1)
{
    m_threadPool.Run();
}

Logger::~Logger()
{
    FinishLogging();
    close(m_logFD);
}

void Logger::Log(std::string msg_)
{
    ThreadPool::SPTask_t newEntry(new details::LogTask(m_logFD, msg_));
    m_threadPool.Add(newEntry);
}

Logger::fd_t Logger::InitLogFile()
{
    fd_t logFd = open(m_path.c_str(), O_RDWR | O_APPEND);
    if(-1 == logFd)
    {
        throw(FileSystemError());
    }

    return logFd;
}

char *Logger::InitPath()
{
    return getenv("LOGGER_FILE_PATH");
}

void Logger::FinishLogging()
{
    ThreadPool::FutureTask<bool> *lastTask = new ThreadPool::FutureTask<bool>(
                                        boost::bind(&Logger::LastTask, this));
    ThreadPool::SPTask_t lastEntry(lastTask);
    
    m_threadPool.Add(lastEntry, ThreadPool::LOW);

    bool status = lastTask->GetReturnVal();
    if(true == status)
    {
        m_threadPool.SetNumOfThreads(0);
    }
}

} // namespace ilrd