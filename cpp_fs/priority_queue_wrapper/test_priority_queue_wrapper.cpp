#include <iostream>
#include <queue>
#include <boost/chrono/duration.hpp>

#include "priority_queue_wrapper.hpp"

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

void TestPQWrapper();
void PrintTestResult(int result);

int main()
{
    TestPQWrapper();

    return 0;
}

void TestPQWrapper()
{
    std::cout << "TestPQWrapper: ";

    PQWrapper<int> pq;
    int a = 3;
    int b = 2;
    int c = 3;

    pq.push(a);
    pq.push(b);
    pq.push(c);

    int d = pq.front();
    int result = (d != a);

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