#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>

#include "logger.hpp"
#include "../singleton/singleton.hpp"

using namespace ilrd;

void TestMoltiThreadedLog();
void TestThreadFunc();

int main()
{
    TestMoltiThreadedLog();
    return 0;
}

void TestThreadFunc()
{
    Logger& log = Singleton<Logger>::GetInstance();
    log.Log("blah");
}

void TestMoltiThreadedLog()
{
    setenv("LOGGER_FILE_PATH", "./log.txt", 0);
    Logger& log = Singleton<Logger>::GetInstance();
    sleep(1);
    log.Log("TestMoltiThreadedLog:");

    boost::thread *threads[5];

    for(size_t i = 0; 5 > i; ++i)
    {
        threads[i] = new boost::thread(&TestThreadFunc);
    }
    for(size_t i = 0; 5 > i; ++i)
    {
        threads[i]->join();
        delete threads[i];
    }
    unsetenv("LOGGER_FILE_PATH");
}

