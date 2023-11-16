#include <iostream>

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>


#include "input_mediator.hpp"
#include "../thread_pool/thread_pool.hpp"
#include "../singleton/singleton.hpp"
#include "../factory/factory.hpp"
#include "dummy_framework.hpp"

using namespace ilrd;

enum PRINT_COLOR
{
    RESET_STYLE = 0,
    RED_COLOR = 31,
    GREEN_COLOR,
    YELLOW_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR
};

class DummyProxy : public Proxy
{
public:
    explicit DummyProxy(int fd_);
    virtual ~DummyProxy();
    virtual boost::shared_ptr<Data> ParseData(Fd_t fd_);
private:
    int m_fd;
};

class DummyCommand : public Command
{
public:
    explicit DummyCommand();
    virtual ~DummyCommand() noexcept;
    virtual void Run();
};



InputMediator::cmd_t CreatorFunc(boost::shared_ptr<Data> data_);

void TestIMSimple();
void TestIMRemoveTask();
void TestIMWithTask();
void TestDynamicLibrary();
void PrintTestResult(int result);


int g_checker;

int main()
{
    TestIMSimple();
    TestIMRemoveTask();
    TestIMWithTask();
    TestDynamicLibrary();

    return 0;
}

void TestIMSimple()
{
    std::cout << "TestIMSimple:" << std::endl;
    ThreadPool *tP = new ThreadPool(0);
    InputMediator iM(&CreatorFunc, tP);
    
    int fds[2];
    pipe(fds);
    iM.ReactorStart();
    
    boost::shared_ptr<Proxy> proxy(new DummyProxy(fds[0]));
    iM.Subscribe(proxy, fds[0]);
    sleep(1);
    
    write(fds[1], "blah...", 8);
    sleep(1);
    int result = (2 != g_checker);
    g_checker = 0;

    iM.ReactorStop();
    sleep(2);

    close(fds[0]);
    close(fds[1]);

    delete tP;

    PrintTestResult(result);
}

void TestIMRemoveTask()
{
    std::cout << "TestIMRemoveTask:" << std::endl;
    ThreadPool *tP = new ThreadPool(0);
    InputMediator iM(&CreatorFunc, tP);
    
    int fds[2];
    pipe(fds);
    iM.ReactorStart();
    
    boost::shared_ptr<Proxy> proxy(new DummyProxy(fds[0]));
    iM.Subscribe(proxy, fds[0]);
    sleep(1);
    
    write(fds[1], "blah...", 8);
    sleep(1);
    int result = (2 != g_checker);
    g_checker = 0;

    iM.Unsubscribe(fds[0]);
    sleep(1);
    
    write(fds[1], "blah...", 8);
    sleep(1);
    result += (0 != g_checker);

    iM.ReactorStop();
    sleep(2);

    close(fds[0]);
    close(fds[1]);

    delete tP;

    PrintTestResult(result);
}

void TestIMWithTask()
{
    std::cout << "TestIMWithTask:" << std::endl;
    
    ThreadPool *tP = new ThreadPool(1);
    tP->Run();

    InputMediator iM(&CreatorFunc, tP);
    
    int fds[2];
    pipe(fds);
    iM.ReactorStart();
    
    boost::shared_ptr<Proxy> proxy(new DummyProxy(fds[0]));
    iM.Subscribe(proxy, fds[0]);
    sleep(1);
    
    write(fds[1], "blah...", 8);
    sleep(1);
    int result = (4 != g_checker);
    g_checker = 0;

    iM.ReactorStop();
    sleep(2);

    close(fds[0]);
    close(fds[1]);

    delete tP;

    PrintTestResult(result);
}

void TestDynamicLibrary()
{
    std::cout << "TestIMWithTask:" << std::endl;
    FrameWork& f = Singleton<FrameWork>::GetInstance();

    void *ptr = dlopen("./libfoo.so", RTLD_LAZY);
    int result = (NULL == ptr);
    dlclose(ptr);

    PrintTestResult(result);
}


InputMediator::cmd_t CreatorFunc(boost::shared_ptr<Data> data_)
{
    if(0 == strcmp("success!", data_->s_str.c_str()))
    {
        g_checker += 1;
    }

    return InputMediator::cmd_t(new DummyCommand);
}

DummyProxy::DummyProxy(int fd_)
: m_fd(fd_)
{
    // Empty on purpose.
}

DummyProxy::~DummyProxy()
{
    // Empty on purpose.
}

boost::shared_ptr<Data> DummyProxy::ParseData(InputMediator::Fd_t fd_)
{
    if(fd_ == m_fd)
    {
        g_checker += 1;
    }
    char buffer[256] = {0};
    read(fd_, buffer, 256);
    if(0 == strcmp("blah...", buffer))
    {
        g_checker += 1;
    }
    
    boost::shared_ptr<Data> data(new Data);
    data->s_str = "success!";

    return data;
}


DummyCommand::DummyCommand()
{
    // Empty on purpose.
}

DummyCommand::~DummyCommand() noexcept
{
    // Empty on purpose.
}

void DummyCommand::Run()
{
    g_checker += 1;
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