#include <string>
#include <iostream>

#include <boost/shared_ptr.hpp>

#include <stdlib.h>                         // strtod()
#include <sys/types.h>                      // open()
#include <sys/stat.h>                       // open()
#include <fcntl.h>                          // open()
#include <unistd.h>                         // close(), write(), pipe()
#include <stdio.h>                          // sprintf()

#include "input_mediator.hpp"
#include "../factory/factory.hpp"
#include "../singleton/singleton.hpp"
#include "dummy_framework.hpp"

namespace ilrd
{
void __attribute__((constructor)) Constructor();

InputMediator::cmd_t FooCallable(boost::shared_ptr<void> data_);
int Add(int x, int y);
int Substract(int x, int y);


class FooCommand : public Command
{
public:
    explicit FooCommand(int x_, int y_, char sign_, 
                        boost::function<int(int,int)> fooFunc_);
    virtual ~FooCommand() noexcept;
    virtual void Run();
private:
    int m_x;
    int m_y;
    char m_sign;
    boost::function<int(int,int)> m_fooFunc;
};

FooCommand::FooCommand(int x_, int y_, char sign_, 
                        boost::function<int(int,int)> fooFunc_)
: m_x(x_)
, m_y(y_)
, m_sign(sign_)
, m_fooFunc(fooFunc_)
{
    // Empty on purpose.
}

FooCommand::~FooCommand() noexcept
{
    // Empty on purpose.
}

void FooCommand::Run()
{
    std::cout << m_x << " " << m_sign << " " << m_y << " = " 
                << m_fooFunc(m_x, m_y) << std::endl;
}

int Add(int x, int y)
{
    return x + y;
}

int Substract(int x, int y)
{
    return x - y;
}

struct FooData
{
    int s_x;
    int s_y;
    char s_sign;
};

class FooProxy : public Proxy
{
public:
    virtual ~FooProxy();
    virtual boost::shared_ptr<Data> ParseData(Fd_t fd_);
};

void Constructor()
{
    std::cout << "hello!" << std::endl;
    FrameWork& f = Singleton<FrameWork>::GetInstance();
    f.m_factory.Add("foo", &FooCallable);

    int pipeFds[2] = {0};
    int status1 = pipe(pipeFds);
    if(-1 == status1)
    {

    }
    boost::shared_ptr<Proxy> fooProxy(new FooProxy());
    
    f.m_iM.Subscribe(fooProxy, pipeFds[0]);
    sleep(1);

    ssize_t status = write(pipeFds[1], "1+2", 4);
    if(-1 == status)
    {

    }
    sleep(1);
    
    status = write(pipeFds[1], "3-2", 4);
    if(-1 == status)
    {

    }
    sleep(1);

    f.m_iM.Unsubscribe(pipeFds[0]);
    sleep(1);

    close(pipeFds[0]);
    close(pipeFds[1]);
}

InputMediator::cmd_t FooCallable(boost::shared_ptr<void> data_)
{
    FooData *fooData = reinterpret_cast<FooData *>(data_.get());
    InputMediator::cmd_t cmd;
    if('+' == fooData->s_sign)
    {
        cmd.reset(new FooCommand(fooData->s_x, fooData->s_y, fooData->s_sign,
                                    &Add));
    }
    else
    {
        cmd.reset(new FooCommand(fooData->s_x, fooData->s_y, fooData->s_sign,
                                    &Substract));
    }

    return cmd;
}


FooProxy::~FooProxy()
{
    // Empty on purpose.
}

boost::shared_ptr<Data> FooProxy::ParseData(Fd_t fd_)
{
    char buffer[256] = {0};
    ssize_t status = read(fd_, buffer, 256);
    FooData *fooData = new FooData;

    fooData->s_x = (int)(buffer[0] - '0');
    fooData->s_sign = buffer[1];
    fooData->s_y = (int)(buffer[2] - '0');
    boost::shared_ptr<Data> data(new Data);
    data->s_data.reset(fooData);
    data->s_str = "foo";

    return data;
}

} // namespace ilrd