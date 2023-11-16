
#include "logger.hpp"
#include "../singleton/singleton.hpp"

using namespace ilrd;

void TestCreateAndLog();

int main()
{
    TestCreateAndLog();
    return 0;
}

void TestCreateAndLog()
{
    setenv("LOGGER_FILE_PATH", "./log.txt", 0);
    Logger& log = Singleton<Logger>::GetInstance();
    log.Log("TestCreateAndLog:");
    for(int i = 0; 5 > i; ++i)
    {
        log.Log("blah");
    }
    unsetenv("LOGGER_FILE_PATH");
}