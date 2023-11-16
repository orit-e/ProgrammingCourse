/*******************************************************************************
 * 																			   *
 * 					File name:			complex_imp.hpp					       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Elia								   *
 * 																			   *
 * ****************************************************************************/

#ifndef ILRD_RD9315_COMPLEX_IMP_H
#define ILRD_RD9315_COMPLEX_IMP_H

#include "complex.hpp"

namespace ilrd
{

inline Complex::Complex(double real_, double imag_)
: m_real(real_)
, m_imag(imag_)
{
    ; // Empty on purpose.
}

inline double Complex::GetReal() const
{
    return m_real;
}

inline double Complex::GetImaginary() const
{
    return m_imag;
}

inline void Complex::SetReal(double real_)
{
    m_real = real_;
}

inline void Complex::SetImaginary(double imag_)
{
    m_imag = imag_;
}

inline Complex& Complex::operator+=(const Complex& cmplx_)
{
    m_real = m_real + cmplx_.m_real;
    m_imag = m_imag + cmplx_.m_imag;

    return *this;
}

inline Complex& Complex::operator-=(const Complex& cmplx_)
{
    m_real = m_real - cmplx_.m_real;
    m_imag = m_imag - cmplx_.m_imag;

    return *this;
}

inline Complex& Complex::operator*=(const Complex& cmplx_)
{
    double real1 = m_real;
    double imag1 = m_imag;
    double real2 = cmplx_.GetReal();
    double imag2 = cmplx_.GetImaginary();

    m_real = real1 * real2 - imag1 * imag2;
    m_imag = real1 * imag2 + imag1 * real2;

    return *this;
}

inline Complex& Complex::operator/=(const Complex& cmplx_)
{
    double real1 = m_real;
    double imag1 = m_imag;
    double real2 = cmplx_.GetReal();
    double imag2 = cmplx_.GetImaginary();

    double tmpBase = (real2 * real2 + imag2 * imag2);

    m_real = (real1 * real2 + imag1 * imag2) / tmpBase;
    m_imag = (imag1 * real2 - real1 * imag2) / tmpBase;

    return *this;
}

inline const Complex operator+(const Complex& cmplx1_, const Complex& cmplx2_)
{
    Complex temp(cmplx1_);
    
    temp += cmplx2_;

    return temp;
}

inline const Complex operator-(const Complex& cmplx1_, const Complex& cmplx2_)
{
    Complex temp(cmplx1_);
    
    temp -= cmplx2_;

    return temp;
}

inline const Complex operator*(const Complex& cmplx1_, const Complex& cmplx2_)
{
    Complex temp(cmplx1_);
    
    temp *= cmplx2_;

    return temp;
}

inline const Complex operator/(const Complex& cmplx1_, const Complex& cmplx2_)
{
    Complex temp(cmplx1_);
    
    temp /= cmplx2_;

    return temp;
}

inline bool operator==(const Complex& cmplx1_, const Complex& cmplx2_)
{
    return ((cmplx1_.GetReal() == cmplx2_.GetReal()) && 
            (cmplx1_.GetImaginary() == cmplx2_.GetImaginary()));
}

inline bool operator!=(const Complex& cmplx1_, const Complex& cmplx2_)
{
    return !(cmplx1_ == cmplx2_);
}

} // namespace ilrd

#endif // ILRD_RD9315_COMPLEX_IMP_H