// standard includes:
#include <iostream>     // std::out, std::endl

// 3rd party includes:
#include <boost/make_shared.hpp>    // boost::make_shared< >()

// local includes:
#include "../singleton/singleton.hpp"
#include "../framework/framework.hpp"

// proxy class:

class MyProxy : public ilrd::Proxy
{
public:
    MyProxy(std::string str)
    : ilrd::Proxy()
    , m_str(str)
    {
        // empty on purpose
    }

    virtual ~MyProxy() noexcept
    {
        // empty on purpose
    }
    
    virtual boost::shared_ptr<ilrd::Data> ParseData(Fd_t fd_)
    {
        char buf[256] = {0};
        read(fd_, buf, 256);
        std::string str(buf);

        boost::shared_ptr<ilrd::Data> myData(new ilrd::Data);
        myData->s_str = m_str;
        myData->s_data = boost::make_shared<std::string>(str);

        (void)fd_;
        return myData;
    }

private:
    std::string m_str;
};

// command class:

class MyCmd : public ilrd::Command
{
public:
    MyCmd(boost::shared_ptr<void> data_)
    : m_toPrint(GetString(data_))
    {
        // empty on purpose
    }

    virtual ~MyCmd() noexcept
    {
        // empty on purpose
    }

    void Run()
    {
        std::cout << "Running MyCmd: " << m_toPrint << std::endl;
        sleep(1);
    }

private:
    std::string m_toPrint;

    std::string GetString(boost::shared_ptr<void> data_)
    {
        return *(static_cast<std::string*>(data_.get()));
    }
};

// command creators:

boost::shared_ptr<ilrd::Command> CmdCreate1(boost::shared_ptr<void> ptr)
{
    return boost::shared_ptr<ilrd::Command>(new MyCmd(ptr));
}

// attribute Ctor && Dtor

void Init() __attribute__((constructor));
void Fini() __attribute__((destructor));

static int fifoFd = 0;

void Init()
{
    ilrd::Framework& fw = ilrd::Singleton<ilrd::Framework>::GetInstance();

    if (0 != mkfifo("./test_lib.fifo", 0666))
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");

            return;
        }
    }

    fifoFd = open("./test_lib.fifo", 0666 | O_RDWR);
    boost::shared_ptr<ilrd::Proxy> mp(new MyProxy("Foo"));
    
    fw.InputMediatorRegister(mp, fifoFd);
    fw.FactoryRegister("Foo", &CmdCreate1);

    std::cout << "load success!" << std::endl;
}

void Fini()
{
    /*
    ilrd::Framework& fw = ilrd::Singleton<ilrd::Framework>::GetInstance();

    std::cout << &fw << std::endl;

    if (&fw != reinterpret_cast<ilrd::Framework *>(0xDEADBEEF))
    {
        // fw.DeRegister(fifoFd);
    }
    */

    //close(fifoFd);
    

    std::cout << "unload success!" << std::endl;
}
