#include <iostream>
#include <cstring>

#include "simple_string.hpp"

using namespace ilrd;

void TestLength();
void TestCStr();
void TestCCtor();
void TestAssignmentOp();
void TestEqualOp();
void TestLargerOpAndSmallerOp();
void TestLeftShiftOp();
void TestRightShiftOp();

void PrintTestResult(int result);

int main()
{
    TestLength();
    TestCStr();
    TestCCtor();
    TestAssignmentOp();
    TestEqualOp();
    TestLargerOpAndSmallerOp();
    TestLeftShiftOp();
    TestRightShiftOp();

    return 0;
}

void TestLength()
{
    int result = 0;

    std::cout << "TestLength: ";

    String s1("Hello world!");
    result += (12 != s1.Length());
    
    String s2("");
    result += (0 != s2.Length());

    String s3;
    result += (0 != s3.Length());

    String s4 = "Hello world!";
    result += (12 != s4.Length());

    PrintTestResult(result);
}

void TestCStr()
{
    int result = 0;

    std::cout << "TestCStr: ";

    String s1("Hello world!");
    result += (0 != strcmp("Hello world!", s1.CStr()));

    String s2("");
    result += (0 != strcmp("", s2.CStr()));

    String s3;
    result += (0 != strcmp("", s3.CStr()));

    String s4 = "Hello world!";
    result += (0 != strcmp("Hello world!", s4.CStr()));

    PrintTestResult(result);
}

void TestCCtor()
{
    int result = 0;
    std::cout << "TestCCtor: ";

    String s1("Hello world!");
    String s2(s1);
    result += (s1.Length() != s2.Length());
    result += (0 != strcmp(s1.CStr(), s2.CStr()));
    result += (s1.CStr() == s2.CStr());

    String s3;
    String s4(s3);
    result += (s3.Length() != s4.Length());
    result += (s3.CStr() == s4.CStr());

    PrintTestResult(result);
}

void TestAssignmentOp()
{
    int result = 0;
    std::cout << "TestAssignmentOp: ";

    String s1("Hello world!");
    String s2 = s1;
    result += (s1.Length() != s2.Length());
    result += (0 != strcmp(s1.CStr(), s2.CStr()));
    result += (s1.CStr() == s2.CStr());

    String s3;
    String s4 = s3;
    result += (s3.Length() != s4.Length());
    result += (s3.CStr() == s4.CStr());

    PrintTestResult(result);
}

void TestEqualOp()
{
    const char *test_str = "Hello world!";
    int result = 0;
    std::cout << "TestEqualOp: ";

    String s1("Hello world!");
    String s2("Goodbye world!");
    String s3;

    String s4 = s1;

    result += (s1 == s2);
    result += (s1 == s3);
    result += !(s1 == s4);

    result += !(test_str == s1);
    result += !(s1 == test_str);
    result += (test_str == s2);
    result += (s2 == test_str);
    
    PrintTestResult(result);
}

void TestLargerOpAndSmallerOp()
{
    const char *test_str = "Hello world!";
    int result = 0;
    std::cout << "TestEqualOp: ";

    String s1("Hello world!");
    String s2("Goodbye world!");
    String s3;

    String s4 = s1;

    result += (s1 < s2);
    result += (s1 < s3);
    result += (s1 < s4);

    result += (test_str < s1);
    result += (s1 < test_str);
    result += (test_str < s2);
    result += !(s2 < test_str);
    
    result += !(s1 > s2);
    result += !(s1 > s3);
    result += (s1 > s4);

    result += (test_str > s1);
    result += (s1 > test_str);
    result += !(test_str > s2);
    result += (s2 > test_str);

    PrintTestResult(result);
}

void TestLeftShiftOp()
{
    std::cout << std::endl;
    std::cout << "TestLeftShiftOp:" << std::endl;

    String s1("Hello world!");

    std::cout << "s1 constructed with string: \"Hello world!\"" << std::endl;
    std::cout << "<< s1: " << s1 << std::endl;
}

void TestRightShiftOp()
{
    std::cout << std::endl;
    std::cout << "TestRightShiftOp:" << std::endl;

    String s1;
    std::cout << "Please insert test string:" << std::endl;
    std::cin >> s1;
    std::cout << "Now s1 string is: " << s1 << std::endl;
}

void PrintTestResult(int result)
{
    (0 == result) ? 
    std::cout << "success!" << std::endl :
    std::cout << "test failed with " << result << " errors." << std::endl;
}
