#include <iostream>

#include "shared_ptr.hpp"

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

struct TestStructBasic
{
    int s_i;
    float s_f;
};

struct TestStructDerived : public TestStructBasic
{
    size_t s_st;
};

void TestSharedPtrBasic();
void TestSharedPtrStarDereference();
void TestSharedPtrArrowDereference();
void TestCopyConstructFromDerivedClass();
void PrintTestResult(int result);

int main()
{
    TestSharedPtrBasic();
    TestSharedPtrStarDereference();
    TestSharedPtrArrowDereference();
    TestCopyConstructFromDerivedClass();

    return 0;
}

void TestSharedPtrBasic()
{
    std::cout << "Test SharedPtr basic functions + get(): ";

    int a = 5;
    int *ptrA = &a;
    SharedPtr<int> sharedPtrA(ptrA);

    int result = (sharedPtrA.get() != ptrA);

    SharedPtr<int> SharedPtrCopy(sharedPtrA);
    result += (SharedPtrCopy.get() != sharedPtrA.get());

    int b = 6;
    int *ptrB = &b;
    SharedPtr<int> sharedPtrB(ptrB);

    result += (sharedPtrB.get() != ptrB);

    SharedPtrCopy = sharedPtrB;
    result += (SharedPtrCopy.get() != sharedPtrB.get());
    result += (SharedPtrCopy.get() == sharedPtrA.get());

    PrintTestResult(result);
}

void TestSharedPtrStarDereference()
{
    std::cout << "Test SharedPtr operator*: ";

    int a = 5;
    int *ptrA = &a;
    SharedPtr<int> sharedPtrA(ptrA);

    int result = (*sharedPtrA != *ptrA);

    PrintTestResult(result);
}

void TestSharedPtrArrowDereference()
{
    std::cout << "Test SharedPtr operator->: ";

    TestStructBasic a = {5, 6.3};
    TestStructBasic *ptrA = &a;
    SharedPtr<TestStructBasic> sharedPtrA(ptrA);

    int result = (sharedPtrA->s_i != ptrA->s_i);
    result += (sharedPtrA->s_f != ptrA->s_f);

    PrintTestResult(result);
}

void TestCopyConstructFromDerivedClass()
{
    std::cout << "Test SharedPtr cctor from derived class to base class: ";
    
    TestStructDerived a;
    TestStructDerived *ptrA = &a;
    SharedPtr<TestStructDerived> sharedPtrA(ptrA);

    SharedPtr<TestStructBasic> sharedPtrBasic(sharedPtrA);

    int result = (sharedPtrBasic.get() != sharedPtrA.get());

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