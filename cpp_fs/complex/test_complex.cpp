/*******************************************************************************
 * 																			   *
 * 					File name:			test_complex.cpp					   *
 * 					Written by:			Orit								   *
 * 																			   *
 * ****************************************************************************/
#include <iostream>

#include "complex.hpp"
#include "complex_imp.hpp"

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

void TestGetRealAndGetImaginary();
void TestSetRealAndSetImaginary();
void TestOpPlus();
void TestOpMinus();
void TestOpMultiply();
void TestOpDivide();
void TestOpEqualAndOpNotEqual();
void TestCompoundOpPlus();
void TestCompoundOpMinus();
void TestCompoundOpMultiply();
void TestCompoundOpDivide();


void TestLeftShiftOp();
void TestRightShiftOp();

void PrintTestResult(int result);

int main()
{
    TestGetRealAndGetImaginary();
    TestSetRealAndSetImaginary();

    TestOpPlus();
    TestOpMinus();
    TestOpMultiply();
    TestOpDivide();
    
    TestOpEqualAndOpNotEqual();
    
    TestCompoundOpPlus();
    TestCompoundOpMinus();
    TestCompoundOpMultiply();
    TestCompoundOpDivide();
    
    TestLeftShiftOp();
    TestRightShiftOp();

    return 0;
}

void TestGetRealAndGetImaginary()
{
    int result = 0;

    std::cout << "TestGetRealAndGetImaginary: ";

    Complex cmplx1(1, 2);
    result += (1 != cmplx1.GetReal());
    result += (2 != cmplx1.GetImaginary());
    
    Complex cmplx2(1.3);
    result += (1.3 != cmplx2.GetReal());
    result += (0 != cmplx2.GetImaginary());

    Complex cmplx3;
    result += (0 != cmplx3.GetReal());
    result += (0 != cmplx3.GetImaginary());

    Complex cmplx4 = -1;
    result += (-1 != cmplx4.GetReal());
    result += (0 != cmplx4.GetImaginary());

    Complex cmplx5(cmplx1);
    result += (1 != cmplx5.GetReal());
    result += (2 != cmplx5.GetImaginary());

    Complex cmplx6 = cmplx1;
    result += (1 != cmplx6.GetReal());
    result += (2 != cmplx6.GetImaginary());

    PrintTestResult(result);
}

void TestSetRealAndSetImaginary()
{
    int result = 0;

    std::cout << "TestSetRealAndSetImaginary: ";

    Complex cmplx(1, 2);
    
    cmplx.SetReal(3);
    result += (3 != cmplx.GetReal());
    result += (2 != cmplx.GetImaginary());

    cmplx.SetImaginary(4.5);
    result += (3 != cmplx.GetReal());
    result += (4.5 != cmplx.GetImaginary());

    PrintTestResult(result);
}

void TestOpPlus()
{
    int result = 0;
    std::cout << "TestOpPlus: ";

    Complex cmplx1(1, 2);
    Complex cmplx2(3, 4);
    Complex cmplx3(-1, -2);
    Complex cmplx4(0.5, -3.7);
    Complex cmplx5;
    Complex testcmplx;

    testcmplx = cmplx1 + cmplx2;
    result += ((cmplx1.GetReal() + cmplx2.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() + cmplx2.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx1 + cmplx3;
    result += ((cmplx1.GetReal() + cmplx3.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() + cmplx3.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx1 + cmplx4;
    result += ((cmplx1.GetReal() + cmplx4.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() + cmplx4.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx1 + cmplx5;
    result += ((cmplx1.GetReal() + cmplx5.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() + cmplx5.GetImaginary()) != 
                testcmplx.GetImaginary());

    PrintTestResult(result);
}

void TestOpMinus()
{
    int result = 0;
    std::cout << "TestOpMinus: ";

    Complex cmplx1(1, 2);
    Complex cmplx2(3, 4);
    Complex cmplx3(-1, -2);
    Complex cmplx4(0.5, -3.7);
    Complex cmplx5;
    Complex testcmplx;

    testcmplx = cmplx1 - cmplx2;
    result += ((cmplx1.GetReal() - cmplx2.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() - cmplx2.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx1 - cmplx3;
    result += ((cmplx1.GetReal() - cmplx3.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() - cmplx3.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx1 - cmplx4;
    result += ((cmplx1.GetReal() - cmplx4.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() - cmplx4.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx1 - cmplx5;
    result += ((cmplx1.GetReal() - cmplx5.GetReal()) != testcmplx.GetReal());
    result += ((cmplx1.GetImaginary() - cmplx5.GetImaginary()) != 
                testcmplx.GetImaginary());

    testcmplx = cmplx5 - cmplx1;
    result += ((cmplx5.GetReal() - cmplx1.GetReal()) != testcmplx.GetReal());
    result += ((cmplx5.GetImaginary() - cmplx1.GetImaginary()) != 
                testcmplx.GetImaginary());

    PrintTestResult(result);
}

void TestOpMultiply()
{
    int result = 0;
    std::cout << "TestOpMultiply: ";

    Complex cmplx1(1, 2);
    Complex cmplx2(3, 4);
    Complex cmplx3(-1, -2);
    Complex cmplx4(0.5, -3.7);
    Complex cmplx5;
    Complex testcmplx;
    double testReal = 0;
    double testImag = 0;

    testcmplx = cmplx1 * cmplx2;
    testReal = (cmplx1.GetReal() * cmplx2.GetReal() - 
                cmplx1.GetImaginary() * cmplx2.GetImaginary());
    testImag = (cmplx1.GetReal() * cmplx2.GetImaginary() + 
                cmplx1.GetImaginary() * cmplx2.GetReal());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    testcmplx = cmplx1 * cmplx3;
    testReal = (cmplx1.GetReal() * cmplx3.GetReal() - 
                cmplx1.GetImaginary() * cmplx3.GetImaginary());
    testImag = (cmplx1.GetReal() * cmplx3.GetImaginary() + 
                cmplx1.GetImaginary() * cmplx3.GetReal());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    testcmplx = cmplx1 * cmplx4;
    testReal = (cmplx1.GetReal() * cmplx4.GetReal() - 
                cmplx1.GetImaginary() * cmplx4.GetImaginary());
    testImag = (cmplx1.GetReal() * cmplx4.GetImaginary() + 
                cmplx1.GetImaginary() * cmplx4.GetReal());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    testcmplx = cmplx1 * cmplx5;
    testReal = (cmplx1.GetReal() * cmplx5.GetReal() - 
                cmplx1.GetImaginary() * cmplx5.GetImaginary());
    testImag = (cmplx1.GetReal() * cmplx5.GetImaginary() + 
                cmplx1.GetImaginary() * cmplx5.GetReal());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    PrintTestResult(result);
}

void TestOpDivide()
{
    int result = 0;
    std::cout << "TestOpDivide: ";

    Complex cmplx1(1, 2);
    Complex cmplx2(3, 4);
    Complex cmplx3(-1, -2);
    Complex cmplx4(0.5, -3.7);
    Complex cmplx5;
    Complex testcmplx;
    double testReal = 0;
    double testImag = 0;

    testcmplx = cmplx1 / cmplx2;
    testReal = (cmplx1.GetReal() * cmplx2.GetReal() + 
                cmplx1.GetImaginary() * cmplx2.GetImaginary()) / 
                (cmplx2.GetReal() * cmplx2.GetReal() + 
                cmplx2.GetImaginary() * cmplx2.GetImaginary());
    testImag = (cmplx1.GetImaginary() * cmplx2.GetReal() - 
                cmplx1.GetReal() * cmplx2.GetImaginary()) /
                (cmplx2.GetReal() * cmplx2.GetReal() + 
                cmplx2.GetImaginary() * cmplx2.GetImaginary());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    testcmplx = cmplx1 / cmplx3;
    testReal = (cmplx1.GetReal() * cmplx3.GetReal() + 
                cmplx1.GetImaginary() * cmplx3.GetImaginary()) / 
                (cmplx3.GetReal() * cmplx3.GetReal() + 
                cmplx3.GetImaginary() * cmplx3.GetImaginary());
    testImag = (cmplx1.GetImaginary() * cmplx3.GetReal() - 
                cmplx1.GetReal() * cmplx3.GetImaginary()) /
                (cmplx3.GetReal() * cmplx3.GetReal() + 
                cmplx3.GetImaginary() * cmplx3.GetImaginary());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    testcmplx = cmplx1 / cmplx4;
    testReal = (cmplx1.GetReal() * cmplx4.GetReal() + 
                cmplx1.GetImaginary() * cmplx4.GetImaginary()) / 
                (cmplx4.GetReal() * cmplx4.GetReal() + 
                cmplx4.GetImaginary() * cmplx4.GetImaginary());
    testImag = (cmplx1.GetImaginary() * cmplx4.GetReal() - 
                cmplx1.GetReal() * cmplx4.GetImaginary()) /
                (cmplx4.GetReal() * cmplx4.GetReal() + 
                cmplx4.GetImaginary() * cmplx4.GetImaginary());
    result += (testReal != testcmplx.GetReal());
    result += (testImag != testcmplx.GetImaginary());

    testcmplx = cmplx5 / cmplx1;
    result += (0 != testcmplx.GetReal());
    result += (0 != testcmplx.GetImaginary());
    
    PrintTestResult(result);
}

void TestOpEqualAndOpNotEqual()
{
    int result = 0;
    std::cout << "TestOpEqualAndOpNotEqual: ";

    Complex cmplx1(1, 2);
    Complex cmplx2(3, 4);
    Complex cmplx3(-1, -2);
    Complex cmplx4(0.5, -3.7);
    Complex cmplx5(cmplx1);
    Complex cmplx6(1);

    result += !(cmplx1 == cmplx1);
    result += (cmplx1 == cmplx2);
    result += (cmplx1 == cmplx3);
    result += !(cmplx1 == cmplx5);
    result += (cmplx1 == cmplx6);

    result += (cmplx1 != cmplx1);
    result += !(cmplx1 != cmplx3);
    result += !(cmplx1 != cmplx2);
    result += (cmplx1 != cmplx5);
    result += !(cmplx1 != cmplx6);

    PrintTestResult(result);
}

void TestCompoundOpPlus()
{
    int result = 0;
    std::cout << "TestCompoundOpPlus: ";

    double real1 = 1;
    double imag1 = 2;
    double real2 = 4;
    double imag2 = 3;
    
    Complex cmplx1(real1, imag1);
    Complex cmplx2(real2, imag2);

    cmplx1 += cmplx2;
    result += ((real1 + real2) != cmplx1.GetReal());
    result += ((imag1 + imag2) != cmplx1.GetImaginary());

    PrintTestResult(result);
}

void TestCompoundOpMinus()
{
    int result = 0;
    std::cout << "TestCompoundOpMinus: ";

    double real1 = 1;
    double imag1 = 2;
    double real2 = 4;
    double imag2 = 3;
    
    Complex cmplx1(real1, imag1);
    Complex cmplx2(real2, imag2);

    cmplx1 -= cmplx2;
    result += ((real1 - real2) != cmplx1.GetReal());
    result += ((imag1 - imag2) != cmplx1.GetImaginary());

    PrintTestResult(result);
}

void TestCompoundOpMultiply()
{
    int result = 0;
    std::cout << "TestCompoundOpMultiply: ";

    double real1 = 1;
    double imag1 = 2;
    double real2 = 4;
    double imag2 = 3;
    
    Complex cmplx1(real1, imag1);
    Complex cmplx2(real2, imag2);
    double testReal = 0;
    double testImag = 0;

    cmplx1 *= cmplx2;
    testReal = (real1 * real2 - imag1 * imag2);
    testImag = (real1 * imag2 + imag1 * real2);
    result += (testReal != cmplx1.GetReal());
    result += (testImag != cmplx1.GetImaginary());

    PrintTestResult(result);
}

void TestCompoundOpDivide()
{
    int result = 0;
    std::cout << "TestCompoundOpDivide: ";

    double real1 = 1;
    double imag1 = 2;
    double real2 = 4;
    double imag2 = 3;
    
    Complex cmplx1(real1, imag1);
    Complex cmplx2(real2, imag2);
    double testReal = 0;
    double testImag = 0;

    cmplx1 /= cmplx2;
    testReal = (real1 * real2 + imag1 * imag2) / 
                (real2 * real2 + imag2  * imag2);
    testImag = (imag1 * real2 - real1 * imag2) /
                (real2 * real2 + imag2 * imag2);
    result += (testReal != cmplx1.GetReal());
    result += (testImag != cmplx1.GetImaginary());

    PrintTestResult(result);
}

void TestLeftShiftOp()
{
    std::cout << std::endl;
    std::cout << "TestLeftShiftOp:" << std::endl;

    Complex cmplx1(1, 2);
    std::cout << "cmplx1 was set with " 
              << "\033[" << BLUE_COLOR << "m"
              << "real: 1 and imaginary: 2"
              << "\033[" << RESET_STYLE << "m" << std::endl;
    std::cout << "\"<< cmplx1\" = " 
              << "\033[" << MAGENTA_COLOR << "m"
              << cmplx1 
              << "\033[" << RESET_STYLE << "m" << std::endl;

    Complex cmplx2(1);
    std::cout << "cmplx2 was set with "
              << "\033[" << BLUE_COLOR << "m"
              << "real: 1 and no imaginary"
              << "\033[" << RESET_STYLE << "m" << std::endl;
    std::cout << "\"<< cmplx2\" = " 
              << "\033[" << MAGENTA_COLOR << "m"
              << cmplx2
              << "\033[" << RESET_STYLE << "m" << std::endl;

    Complex cmplx3;
    std::cout << "cmplx1 was set with "
              << "\033[" << BLUE_COLOR << "m"
              << "no real and no imaginary"
              << "\033[" << RESET_STYLE << "m" << std::endl;
    std::cout << "\"<< cmplx3\" = " 
              << "\033[" << MAGENTA_COLOR << "m"
              << cmplx3
              << "\033[" << RESET_STYLE << "m" << std::endl;

    Complex cmplx4(-1, 0.5);
    std::cout << "cmplx4 was set with " 
              << "\033[" << BLUE_COLOR << "m"
              << "real: -1 and imaginary: 0.5"
              << "\033[" << RESET_STYLE << "m" << std::endl;
    std::cout << "\"<< cmplx4\" = " 
              << "\033[" << MAGENTA_COLOR << "m"
              << cmplx4
              << "\033[" << RESET_STYLE << "m" << std::endl;

    Complex cmplx5(7.84, -16.25);
    std::cout << "cmplx4 was set with " 
              << "\033[" << BLUE_COLOR << "m"
              << "real: 7.84 and imaginary: -16.25"
              << "\033[" << RESET_STYLE << "m" << std::endl;
    std::cout << "\"<< cmplx5\" = " 
              << "\033[" << MAGENTA_COLOR << "m"
              << cmplx5
              << "\033[" << RESET_STYLE << "m" << std::endl;
}

void TestRightShiftOp()
{
    std::cout << std::endl;
    std::cout << "TestRightShiftOp:" << std::endl;

    Complex cmplx;
    std::cout << "Please insert test complex number:" << std::endl;
    std::cin >> cmplx;
    std::cout << "Complex number received: " 
              << "\033[" << MAGENTA_COLOR << "m" 
              << cmplx 
              << "\033[" << RESET_STYLE << "m" << std::endl;
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

