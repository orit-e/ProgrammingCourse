
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <iterator>
#include "scope_lock.hpp"

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

static void TestCppMutex();
static void TestWriteCppMutex(size_t index_, std::mutex &mutex_);
void PrintTestResult(int result);

size_t g_arr[5];
size_t g_index;

int main()
{
    TestCppMutex();

    return 0;
}

static void TestCppMutex()
{
    std::cout << "Test ScopeLock with C++ Mutex: ";
    std::mutex myMutex;

    std::vector<std::thread> myThreads;

    for(size_t i = 0; 5 > i; ++i)
    {
        myThreads.push_back(std::thread(TestWriteCppMutex, i, 
                                        std::ref(myMutex)));
    }
    for(std::vector<std::thread>::iterator iter = myThreads.begin(); 
        myThreads.end() != iter; ++iter)
    {
        (*iter).join();
    }

    int result = 0;
    for(size_t i = 0; 5 > i; ++i)
    {
        result += (g_arr[i] != i * 2);
    }

    PrintTestResult(result);
}

static void TestWriteCppMutex(size_t index_, std::mutex &mutex_)
{
    try
    {
        ScopeLock<std::mutex> lock(mutex_);

        g_index = index_;
        size_t tmpVar = index_ * 2;
        g_arr[g_index] = tmpVar;
    }
    catch(std::system_error&)
    {

    }
    
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