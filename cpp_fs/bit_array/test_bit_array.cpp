#include <iostream>

#include "bit_array.hpp"

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


void TestPhaseOne();
void TestBitwiseOperators();
void TestEqualAndNotEqualOp();
void TestSetGetFlip();
void TestCount();
void TestToString();
void PrintTestResult(int result);

int main()
{
    TestPhaseOne();
    TestBitwiseOperators();
    TestEqualAndNotEqualOp();
    TestSetGetFlip();
    TestCount();
    TestToString();

    return 0;
}

void TestPhaseOne()
{
    std::cout << "Test BitArray functions of Phase 1: ";
    
    BitArray<64> a;
    a[3] = 1;

    const BitArray<64> b(a);
    int result = (0 != b[0]);
    result += (1 != b[3]);
    a[16] = b[3];
    result += (1 != a[16]);

    BitArray<70> c;
    result += (0 != c[65]);

    PrintTestResult(result);
}

void TestBitwiseOperators()
{
    std::cout << "Test BitArray operator|=, operator&=, operator^=: ";

    BitArray<64> a;
    a[0] = 1;
    a[5] = 1;

    BitArray<64> b;
    b[0] = 1;
    b[3] = 1;

    b |= a;

    int result = (1 != b[0]);
    result += (0 != b[1]);
    result += (1 != b[3]);
    result += (1 != b[5]);

    a[19] = 1;
    b[0] = 0;

    b &= a;
    result += (0 != b[0]);
    result += (0 != b[1]);
    result += (0 != b[3]);
    result += (1 != b[5]);
    result += (0 != b[19]);

    b[32] = 1;

    b ^= a;
    result += (1 != b[0]);
    result += (0 != b[1]);
    result += (0 != b[3]);
    result += (0 != b[5]);
    result += (1 != b[19]);
    result += (1 != b[32]);

    PrintTestResult(result);
}

void TestEqualAndNotEqualOp()
{
    std::cout << "Test BitArray operator==, operator!=: ";

    BitArray<64> a;
    a[0] = 1;
    a[5] = 1;
    a[19] = 1;
    a[32] = 1;

    BitArray<64> b(a);
    bool boolReturn = (a == b);
    int result = (1 != boolReturn);

    b[53] = 1;

    boolReturn = (a == b);
    result += (0 != boolReturn);

    BitArray<64> c(b);
    boolReturn = (a != c);
    result += (1 != boolReturn);

    boolReturn = (b != c);
    result += (0 != boolReturn);
    
    PrintTestResult(result);
}

void TestSetGetFlip()
{
    std::cout << "Test BitArray set, get, flip: ";

    BitArray<64> a;
    a.Set(1, 0);
    
    int result = (1 != a[0]);

    a.Set(1, 32);
    result += (1 != a[32]);

    result += (1 != a.Get(32));
    result += (0 != a.Get(50));

    a.Set(1);
    result += (1 != a.Get(46));
    result += (1 != a.Get(50));

    a.Set(0, 0);
    result += (0 != a.Get(0));

    a.Set(0, 13);
    result += (0 != a.Get(13));

    a.Flip(46);
    result += (0 != a.Get(46));

    a.Flip(0);
    result += (1 != a.Get(0));

    a.Flip();
    result += (0 != a.Get(0));
    result += (0 != a.Get(2));
    result += (1 != a.Get(13));
    result += (0 != a.Get(32));
    result += (1 != a.Get(46));
    result += (0 != a.Get(50));

    PrintTestResult(result);
}

void TestCount()
{
    std::cout << "Test BitArray count: ";

    BitArray<64> a;
    int result = (0 != a.Count());

    a.Set(1);
    result += (64 != a.Count());

    a.Set(0, 0);
    a.Set(0, 19);
    a.Set(0, 52);
    a.Set(0, 27);

    result += (60 != a.Count());

    BitArray<17> b;
    b[15] = 1;
    b[3] = 1;
    result += (2 != b.Count());

    b.Flip();
    result += (15 != b.Count());

    PrintTestResult(result);
}

void TestToString()
{
    std::cout << "Test BitArray ToString: ";

    BitArray<64> a;
    std::string tmp = a.ToString();
    std::string testString = 
            "0000000000000000000000000000000000000000000000000000000000000000";
    int result = (0 != testString.compare(tmp));

    a.Set(1);
    tmp = a.ToString();
    testString = 
            "1111111111111111111111111111111111111111111111111111111111111111";
    result += (0 != testString.compare(tmp));

    a.Set(0, 0);
    a.Set(0, 19);
    a.Set(0, 52);
    a.Set(0, 27);
    tmp = a.ToString();
    testString = 
            "1111111111101111111111111111111111110111111101111111111111111110";
    result += (0 != testString.compare(tmp));
    
    BitArray<17> b;
    b[15] = 1;
    b[3] = 1;
    tmp = b.ToString();
    testString = 
            "01000000000001000";
    result += (0 != testString.compare(tmp));

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