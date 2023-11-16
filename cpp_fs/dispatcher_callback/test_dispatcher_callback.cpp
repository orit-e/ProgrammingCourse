#include <iostream>
#include <boost/bind.hpp>

#include "dispatcher_callback.hpp"
#include "derived_classes.hpp"

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

void SimpleUpdate1(int& data_);
void UpdateDeathFunc1();
void UpdateDeathFunc2();
void UpdateDeathFunc3();
void TestBroadcastToOneObserver();
void TestBroadcastToTwoObserver();
void TestBroadcastToThreeObserver();
void TestRemoveOneWhileBroadcast();
void TestRemoveAllWhileBroadcast();
void TestAddObserverWhileBroadcast();
void TestAddTwoObserverWhileBroadcast();
void PrintTestResult(int result);

bool g_isDead = false;
int g_selfRemoveChecker;

int main()
{
    TestBroadcastToOneObserver();
    TestBroadcastToTwoObserver();
    TestBroadcastToThreeObserver();
    TestRemoveOneWhileBroadcast();
    TestRemoveAllWhileBroadcast();
    TestAddObserverWhileBroadcast();
    TestAddTwoObserverWhileBroadcast();

    return 0;
}

void TestBroadcastToOneObserver()
{
    std::cout << "TestBroadcastToOneObserver: ";
    DispatcherEnvelop *d = new DispatcherEnvelop;
    SimpleCallback<int, DispatcherEnvelop> c(d, &SimpleUpdate1, 
                                                &UpdateDeathFunc1);

    int data_ = 0;
    d->Broadcast(data_);
    int result = (1 != data_);

    result += (false != g_isDead);
    delete d;
    result += (true != g_isDead);

    PrintTestResult(result);
}

void TestBroadcastToTwoObserver()
{
    std::cout << "TestBroadcastToTwoObserver: ";
    DispatcherEnvelop d;
    SimpleCallback<int, DispatcherEnvelop> c1(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> c2(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);

    int data_ = 0;
    d.Broadcast(data_);
    int result = (2 != data_);

    PrintTestResult(result);
}

void TestBroadcastToThreeObserver()
{
    std::cout << "TestBroadcastToThreeObserver: ";
    DispatcherEnvelop d;
    SimpleCallback<int, DispatcherEnvelop> c1(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> c2(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> c3(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);

    int data_ = 0;
    d.Broadcast(data_);
    int result = (3 != data_);

    PrintTestResult(result);
}

void TestRemoveOneWhileBroadcast()
{
    std::cout << "TestRemoveOneWhileBroadcast: ";
    DispatcherEnvelop d;
    SimpleCallback<int, DispatcherEnvelop> c1(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> *c1Ptr = &c1;
    CallbackRemove<int, SimpleCallback<int, DispatcherEnvelop> *> c2(&d, c1Ptr);
    SimpleCallback<int, DispatcherEnvelop> *c4Ptr;
    CallbackRemove<int, SimpleCallback<int, DispatcherEnvelop> *> c3(&d, c4Ptr);
    SimpleCallback<int, DispatcherEnvelop> c4(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    c4Ptr = &c4;

    int data_ = 0;
    d.Broadcast(data_);
    int result = (3 != data_);

    result += (NULL != c1Ptr);
    result += (NULL != c4Ptr);

    PrintTestResult(result);
}

void TestRemoveAllWhileBroadcast()
{
    std::cout << "TestRemoveAllWhileBroadcast: ";
    DispatcherEnvelop d;
    SimpleCallback<int, DispatcherEnvelop> c1(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc3);
    SimpleCallback<int, DispatcherEnvelop> c2(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc3);

    CallbackBase<int, DispatcherEnvelop> *ptr1 = &c1;
    CallbackBase<int, DispatcherEnvelop> *ptr2 = &c2;
    CallbackBase<int, DispatcherEnvelop> *ptr4 = NULL;
    CallbackBase<int, DispatcherEnvelop> *ptr5 = NULL;
    std::vector<CallbackBase<int, DispatcherEnvelop>**> toRemove;
    toRemove.push_back(&ptr1);
    toRemove.push_back(&ptr2);
    toRemove.push_back(&ptr4);
    toRemove.push_back(&ptr5);
    CallbackRemoveAll<int> c3(&d, toRemove, g_selfRemoveChecker);
    
    SimpleCallback<int, DispatcherEnvelop> c4(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc3);
    ptr4 = &c4;
    SimpleCallback<int, DispatcherEnvelop> c5(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc3);
    ptr5 = &c5;


    int data = 0;
    d.Broadcast(data);
    int result = (3 != data);

    result += (0 != g_selfRemoveChecker);

    PrintTestResult(result);
}

void TestAddObserverWhileBroadcast()
{
    std::cout << "TestAddObserverWhileBroadcast: ";
    DispatcherEnvelop d;
    SimpleCallback<int, DispatcherEnvelop> c1(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> *c4Ptr = NULL;
    CallBackAdd<int> c2(&d, &c4Ptr, &SimpleUpdate1, &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> c3(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);

    int data_ = 0;
    d.Broadcast(data_);
    int result = (4 != data_);
    result += (NULL == c4Ptr);
    
    delete c4Ptr;

    PrintTestResult(result);
}

void TestAddTwoObserverWhileBroadcast()
{
    std::cout << "TestAddTwoObserverWhileBroadcast: ";
    DispatcherEnvelop d;
    SimpleCallback<int, DispatcherEnvelop> c1(&d, &SimpleUpdate1, 
                                                &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> *c4Ptr = NULL;
    CallBackAdd<int> c2(&d, &c4Ptr, &SimpleUpdate1, &UpdateDeathFunc2);
    SimpleCallback<int, DispatcherEnvelop> *c5Ptr = NULL;
    CallBackAdd<int> c3(&d, &c5Ptr, &SimpleUpdate1, &UpdateDeathFunc2);

    int data_ = 0;
    d.Broadcast(data_);
    int result = (5 != data_);
    result += (NULL == c4Ptr);
    result += (NULL == c5Ptr);
    
    delete c4Ptr;
    delete c5Ptr;

    PrintTestResult(result);
}

void SimpleUpdate1(int& data_)
{
    data_ += 1;
}

void UpdateDeathFunc1()
{
    g_isDead = true;
}

void UpdateDeathFunc2()
{
    // Empty on purpose
}

void UpdateDeathFunc3()
{
    g_selfRemoveChecker += 1;
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