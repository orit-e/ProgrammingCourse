/*******************************************************************************
 * 																			   *
 * 					File name:			logger.hpp		         		       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Yona								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_LOGGER_HPP
#define ILRD_RD9315_LOGGER_HPP

// Standard includes
#include <string>
#include <exception>

// Local includes
#include "../utils/utils.hpp"
#include "../singleton/singleton.hpp"
#include "../thread_pool/thread_pool.hpp"

namespace ilrd
{

class Logger
{
public:
    typedef int fd_t;

    class FileSystemError : public std::exception
    {
    public:
        const char *what() const noexcept;
    };


    ~Logger(); // may throw FileSystemError?
    // Compiler generated
    // Logger(const Logger& other);
    // Logger& operator=(const Logger& other);

    void Log(std::string msg_); // may throw FileSystemError

private:
    friend class Singleton<Logger>;
    explicit Logger(); // may throw FileSystemError

private:
    std::string m_path;
    fd_t m_logFD;
    ThreadPool m_threadPool;

    fd_t InitLogFile();
    char *InitPath();
    void FinishLogging();
    bool LastTask();
};

} // namespace ilrd

#endif // ILRD_RD9315_LOGGER_HPP