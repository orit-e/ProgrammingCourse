#include "singleton.hpp"

#include <iostream>
#include <vector>

#include <boost/thread.hpp>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace ilrd;

class Test
{
public:
    Test()
    : m_i(1)
    , m_c('T')
    , m_ip(new int(2))
    {
        std::cout << "Test Created" << std::endl;
    }
    ~Test()
    {
        delete m_ip;
        std::cout << "Test Destroyed" << std::endl;
    }
    void Print()
    {
        std::cout << m_i << std::endl;
        std::cout << m_c << std::endl;
        std::cout << *m_ip << std::endl;
    }

private:
    int m_i;
    char m_c;
    int *m_ip;
};

void ThreadFunc()
{
    Singleton<Test>::GetInstance();
}

int main()
{
    boost::thread threadArr[8];

    for (size_t i = 0; 8 > i; ++i)
    {
       threadArr[i] = boost::thread(&ThreadFunc);
    }

    Singleton<Test>::GetInstance().Print();

    for (size_t i = 0; 8 > i; ++i)
    {
        threadArr[i].join();
    }

    pid_t pid = 0;

    pid = fork();
    if (-1 == pid)
    {
        std::cout << "fork() failed" << std::endl;
    }

    if (0 == pid)
    {
        return 0;
    }
    else
    {
        wait(NULL);
    }

    return 0;
}
