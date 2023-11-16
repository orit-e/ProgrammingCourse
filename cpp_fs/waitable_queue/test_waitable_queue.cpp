#include <iostream>
#include <queue>
#include <boost/chrono/duration.hpp>

#include "waitable_queue.hpp"

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

void TestCreateWQ();
void TestTimedWait();
void PrintTestResult(int result);

int main()
{
    TestCreateWQ();
    TestTimedWait();

    return 0;
}

void TestCreateWQ()
{
    std::cout << "TestCreateWQ - ctor, IsEmpty, Push, Pop (no wait type): ";

    WaitableQueue< std::queue<int> > myQueue;

    int result = (1 != myQueue.IsEmpty());

    int a = 1;
    int b = 2;
    int c = 3;
    myQueue.Push(a);
    myQueue.Push(b);
    myQueue.Push(c);

    result += (0 != myQueue.IsEmpty());

    int popResult = 0;
    myQueue.Pop(popResult);
    result += (a != popResult);

    myQueue.Pop(popResult);
    result += (b != popResult);

    myQueue.Pop(popResult);
    result += (c != popResult);

    PrintTestResult(result);
}

void TestTimedWait()
{
    using namespace boost::chrono;
    std::cout << "TestTimedWait - Pop (wait type): ";

    WaitableQueue< std::queue<int> > myQueue;
    
    system_clock::time_point timeBefore(system_clock::now());
    
    int popResult = 0;
    int result = (0 != myQueue.Pop(popResult, 
                    static_cast<seconds>(5)));
    result += (static_cast<seconds>(5) > system_clock::now() - timeBefore);
    
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