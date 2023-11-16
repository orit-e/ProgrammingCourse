/******************************************************************************
 *
 *                          worksheet:      rc_string
 *                          filename;       test_rc_string.cpp
 *                          written by:     guy ben-tov
 *                          reviewed by:    tomer baytman
 *                          last edit:      30112020
 *
 *****************************************************************************/

#include <iostream>     // cout, cin
#include <cstring>      // strcmp

#include "rc_string.hpp"

using namespace ilrd;

void TestCCtor(); 
void TestAssignmentOperator(); 
void TestLength();
void TestIsShared();
void TestConcat(); 
void TestBracketsOperator(); 
void TestEqualityOperator(); 
void TestInequalityOperator(); 
void TestGreaterThanOperator(); 
void TestLesserThanOperator(); 
void TestOstream();
void TestIstream();
 
void TestResult(int result);

int main(void)
{
    TestCCtor(); 
    TestAssignmentOperator(); 
    TestLength();
    TestIsShared();
    TestConcat(); 
    TestBracketsOperator(); 

    TestEqualityOperator(); 
    TestInequalityOperator(); 
    TestGreaterThanOperator(); 
    TestLesserThanOperator(); 

    TestOstream();
    TestIstream();
 
    return 0;
}

void TestCCtor()
{
    int result = 4;

    std::cout << "TestCCtor: " << std::endl;

    RCString s1;
    RCString s2("Hello");

    RCString s3(s1);
    RCString s4(s2);

    const char *cSting1 = s1.ToCStr();
    const char *cSting2 = s2.ToCStr();
    const char *cSting3 = s3.ToCStr();
    const char *cSting4 = s4.ToCStr();

    result -= (0 == strcmp(cSting1, cSting3));
    result -= (0 == strcmp("", cSting3));
    result -= (0 == strcmp(cSting2, cSting4));
    result -= (0 == strcmp("Hello", cSting4));

    TestResult(result);
}

void TestAssignmentOperator()
{
    int result = 4;

    std::cout << "TestAssignmentOperator: " << std::endl;

    RCString s1;
    RCString s2("Hello");

    RCString s3 = s1;
    RCString s4 = s2;

    const char *cSting1 = s1.ToCStr();
    const char *cSting2 = s2.ToCStr();
    const char *cSting3 = s3.ToCStr();
    const char *cSting4 = s4.ToCStr();

    result -= (0 == strcmp(cSting1, cSting3));
    result -= (0 == strcmp("", cSting3));
    result -= (0 == strcmp(cSting2, cSting4));
    result -= (0 == strcmp("Hello", cSting4));

    TestResult(result);
}

void TestLength()
{
    int result = 2;

    std::cout << "TestLength: " << std::endl;

    RCString s1;
    RCString s2("Hello");

    result -= (0 == s1.Length());
    result -= (5 == s2.Length());

    TestResult(result);
}

void TestIsShared()
{
    int result = 6;

    std::cout << "TestIsShared: " << std::endl;

    RCString s1;
    RCString s2("Hello");

    result -= (false == s1.IsShared());
    result -= (false == s2.IsShared());

    RCString s3 = s1;
    RCString s4 = s2;

    result -= (true == s1.IsShared());
    result -= (true == s2.IsShared());
    result -= (true == s3.IsShared());
    result -= (true == s4.IsShared());

    TestResult(result);
}

void TestConcat()
{
    int result = 4;

    std::cout << "TestConcat: " << std::endl;

    RCString s1;
    RCString s2("Hello");

    size_t s1_len = s1.Length();
    size_t s2_len = s2.Length();

    s1.Concat("new string!");
    s2.Concat(", World!");

    result -= (0 == strcmp("new string!", s1.ToCStr()));
    result -= (0 == strcmp("Hello, World!", s2.ToCStr()));
    result -= (s1.Length() == s1_len + strlen("new string!"));
    result -= (s2.Length() == s2_len + strlen(", World!"));

    TestResult(result);
}

void TestBracketsOperator()
{
    int result = 10;

    std::cout << "TestBracketsOperator: " << std::endl;

    RCString s1 = "Hello, World!";
    RCString s2(s1);

    result -= (true == s1.IsShared());
    result -= (true == s2.IsShared());

    char s1_char = s1[9];
    char s2_char = s2[9];

    result -= ('r' == s1_char);
    result -= ('r' == s2_char);
    /*
    result -= (true == s1.IsShared());
    result -= (true == s2.IsShared());
    */

    s1[9] = 'u';

    s1_char = s1[9];
    s2_char = s2[9];

    result -= ('u' == s1_char);
    result -= ('r' == s2_char);
    result -= (false == s1.IsShared());
    result -= (false == s2.IsShared());

    result -= (0 == strcmp("Hello, Would!", s1.ToCStr()));
    result -= (0 == strcmp("Hello, World!", s2.ToCStr()));

    TestResult(result);
}

void TestEqualityOperator()
{
    int result = 4;

    std::cout << "TestEqualityOperator: " << std::endl;

    RCString s1;
    RCString s2("Hello");

    RCString s3(s1);
    RCString s4(s2);

    result -= (s1 == s3);
    result -= ("" == s3);
    result -= (s2 == s4);
    result -= ("Hello" == s4);

    TestResult(result);
}

void TestInequalityOperator()
{
    int result = 3;

    std::cout << "TestInequalityOperator: " << std::endl;

    RCString s1("Hello");
    RCString s2("World");

    RCString s3(s1);

    result -= (s1 != s2);
    result -= (s1 == s3);

    s3[0] = 'J';

    result -= (s1 != s3);

    TestResult(result);
}

void TestGreaterThanOperator()
{
    int result = 2;

    std::cout << "TestGreaterThanOperator: " << std::endl;

    RCString s1("Hello");
    RCString s2("World");

    RCString s3(s1);

    result -= (s2 > s1);

    s3[0] = 'F';

    result -= (s1 > s3);

    TestResult(result);
}

void TestLesserThanOperator()
{
    int result = 2;

    std::cout << "TestLesserThanOperator: " << std::endl;

    RCString s1("Hello");
    RCString s2("World");

    RCString s3(s1);

    result -= (s1 < s2);

    s3[0] = 'F';

    result -= (s3 < s1);

    TestResult(result);
}
 
void TestOstream()
{
    std::cout << "TestOstream: " << std::endl;

    RCString s1("Hello world!");

    std::cout << "s1 constructed with string: \"Hello world!\"" << std::endl;
    std::cout << "<< s1: " << s1 << std::endl;
}

void TestIstream()
{
    std::cout << "TestIstream: " << std::endl;

    RCString s1;
    std::cout << "Please insert test string:" << std::endl;
    std::cin >> s1;
    std::cout << "Now s1 string is: " << s1 << std::endl;
}

void TestResult(int result)
{
    (0 == result) ?
    std::cout << "success!" << std::endl :
    std::cout << "test failed with " << result << " errors." << std::endl;
}
