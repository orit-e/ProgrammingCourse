/*******************************************************************************
 * 																			   *
 * 					File name:			complex.cpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Elia								   *
 * 																			   *
 * ****************************************************************************/
#include <iostream> //  ostream, istream, showpos, noshowpos

#include "complex.hpp"
#include "complex_imp.hpp"

namespace ilrd
{

std::ostream& operator<<(std::ostream& os_, const Complex& c_)
{
    return (os_ << c_.GetReal() 
                << std::showpos 
                << c_.GetImaginary() 
                << std::noshowpos << "i");
}

std::istream& operator>>(std::istream& is_, Complex& c_)
{
    double tmpReal = 0.0;
    double tmpImag = 0.0;

    is_ >> tmpReal >> tmpImag;

    c_.SetReal(tmpReal);
    c_.SetImaginary(tmpImag);
    
    return is_;
}

} // namespace ilrd