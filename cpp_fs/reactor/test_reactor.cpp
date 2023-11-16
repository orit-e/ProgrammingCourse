// test_reactor.cpp
// Writer: Dvir Sadeh
// Reviewer:

#include <iostream>
#include <fcntl.h>      // open()
#include <unistd.h>     // sleep(), close(), pipe()
#include <boost/bind.hpp>


#include "reactor.hpp"

enum PRINT_COLOR
{
    RESET_STYLE = 0,
    RED_COLOR = 31,
    GREEN_COLOR,
    YELLOW_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR
};

using namespace ilrd;

void StopIt(Reactor *reactor);
void Foo(Reactor *reactor);
void TestReactorAddStop();
void TestReactorAvailableFd();
void TestReactorIllegalFd();
void TestReactorNonavailableFd();
void TestReactorRemove();

void PrintTestResult(int result);

int g_num;

int main()
{
    TestReactorAddStop();
    TestReactorAvailableFd();
    TestReactorIllegalFd();
    TestReactorNonavailableFd();
    TestReactorRemove();
    
    return 0;
}



/****************** Helper Funcs ******************/

void StopIt(Reactor *reactor)
{
    reactor->Stop();
}

void Foo(Reactor *reactor)
{
    g_num = 1;
    (void)reactor;
}

/************************ Tests ***************************/

void TestReactorAddStop()
{
    std::cout << "TestReactorAddStop: " << std::endl;
    Reactor reactor;
    Reactor::CallbackFunc_t stopIt = boost::bind(&StopIt, &reactor);

    reactor.Add(4, Watcher::REACT_READ, stopIt);
    reactor.Run();

    sleep(1);    
    int result = (1 != reactor.IsStopped());

    PrintTestResult(result);
}

void TestReactorAvailableFd()
{
    std::cout << "TestReactorAvailableFd: " << std::endl;
    Reactor reactor;
    Reactor::CallbackFunc_t stopIt = boost::bind(&StopIt, &reactor);
    Reactor::CallbackFunc_t foo = boost::bind(&Foo, &reactor);

    int fd = open("./file", O_RDONLY);
    int result = (-1 == fd);

    reactor.Add(fd, Watcher::REACT_READ, foo);
    reactor.Add(4, Watcher::REACT_READ, stopIt);
    reactor.Run();
    
    sleep(1);
    result += (1 != g_num);
    
    g_num = 0;
    result += (1 != reactor.IsStopped());

    close(fd);

    PrintTestResult(result);
}

void TestReactorIllegalFd()
{
    std::cout << "TestReactorIllegalFd: " << std::endl;
    Reactor reactor;
    Reactor::CallbackFunc_t stopIt = boost::bind(&StopIt, &reactor);
    Reactor::CallbackFunc_t foo = boost::bind(&Foo, &reactor);

    reactor.Add(-1, Watcher::REACT_READ, foo);
    reactor.Add(4, Watcher::REACT_READ, stopIt);
    reactor.Run();

    sleep(1);    
    int result = (1 != reactor.IsStopped());
    result += (0 != g_num);

    PrintTestResult(result);
}

void TestReactorNonavailableFd()
{
    std::cout << "TestReactorNonavailableFd: " << std::endl;
    Reactor reactor;
    Reactor::CallbackFunc_t stopIt = boost::bind(&StopIt, &reactor);
    Reactor::CallbackFunc_t foo = boost::bind(&Foo, &reactor);

    int pipe_fd[2] = {0};
    pipe(pipe_fd);
    reactor.Add(pipe_fd[0], Watcher::REACT_READ, foo);
    reactor.Add(4, Watcher::REACT_READ, stopIt);
    reactor.Run();

    sleep(1);    
    int result = (1 != reactor.IsStopped());
    result += (1 != g_num);
    g_num = 0;

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    PrintTestResult(result);
}

void TestReactorRemove()
{
    std::cout << "TestReactorRemove: " << std::endl;
    Reactor reactor;
    Reactor::CallbackFunc_t stopIt = boost::bind(&StopIt, &reactor);
    Reactor::CallbackFunc_t foo = boost::bind(&Foo, &reactor);

    reactor.Add(4, Watcher::REACT_READ, foo);
    reactor.Remove(4, Watcher::REACT_READ);
    reactor.Add(4, Watcher::REACT_READ, stopIt);
    reactor.Run();

    sleep(1);    
    int result = (1 != reactor.IsStopped());
    result += (0 != g_num);

    PrintTestResult(result);
}

void PrintTestResult(int result)
{
    (0 == result) ? 
    std::cout << "\033[" << GREEN_COLOR << "m"
              << "success!" 
              << "\033[" << RESET_STYLE << "m"  << std::endl :
    std::cout << "\033[" << RED_COLOR << "m" 
              << "test failed with " << result << " errors." 
              << "\033[" << RESET_STYLE << "m" << std::endl;            
}