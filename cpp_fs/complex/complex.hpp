/*******************************************************************************
 * 																			   *
 * 					File name:			complex.hpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Elia								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_COMPLEX_H
#define ILRD_RD9315_COMPLEX_H

#include <iosfwd>

namespace ilrd
{

class Complex
{
public:
    inline Complex(double real_ = 0.0, double imag_ = 0.0); // Implicit on 
                                                            // purpose.

    inline Complex& operator+=(const Complex& cmplx_);
    inline Complex& operator-=(const Complex& cmplx_);
    inline Complex& operator*=(const Complex& cmplx_);
    inline Complex& operator/=(const Complex& cmplx_);

    inline double GetReal() const;
    inline double GetImaginary() const;

    inline void SetReal(double real_);
    inline void SetImaginary(double imag_);

private:
    double m_real;
    double m_imag;
};


inline const Complex operator+(const Complex& cmplx1_, const Complex& cmplx2_);
inline const Complex operator-(const Complex& cmplx1_, const Complex& cmplx2_);
inline const Complex operator*(const Complex& cmplx1_, const Complex& cmplx2_);
inline const Complex operator/(const Complex& cmplx1_, const Complex& cmplx2_);

inline bool operator==(const Complex& cmplx1_, const Complex& cmplx2_);
inline bool operator!=(const Complex& cmplx1_, const Complex& cmplx2_);

std::ostream& operator<<(std::ostream& os_, const Complex& cmplx_);

std::istream& operator>>(std::istream& is_, Complex& cmplx_);


}

#endif // ILRD_RD9315_COMPLEX_H