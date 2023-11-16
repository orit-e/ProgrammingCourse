/*******************************************************************************
 * 																			   *
 * 					File name:			shapes_imp.hpp					       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		    								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream>

#include "shapes.hpp"

namespace ilrd
{

void Shape::Move(std::size_t offset_) noexcept
{
    m_printOffset = offset_;
}


void Line::Draw() const noexcept
{
    Shape::Draw();
    std::cout << "Line" << std::endl;
}


void Circle::Draw() const noexcept
{
    Shape::Draw();
    std::cout << "Circle" << std::endl;
}


void Rectangle::Draw() const noexcept
{
    Shape::Draw();
    std::cout << "Rectangle" << std::endl;
}


void Square::Draw() const noexcept
{
    Shape::Draw();
    std::cout << "Square" << std::endl;
}

} // namespace ilrd