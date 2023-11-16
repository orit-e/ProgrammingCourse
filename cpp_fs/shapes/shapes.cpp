/*******************************************************************************
 * 																			   *
 * 					File name:			shapes.cpp  					       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		    								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream>
#include <list>

#include "shapes.hpp"

namespace ilrd
{

Shape::Shape() noexcept
: m_printOffset(0)
{
    ; // Empty on purpose.
}

Shape::~Shape() noexcept
{
    ; // Empty on purpose.
}

void Shape::Draw() const noexcept
{
    for(std::size_t i = 0; m_printOffset > i; ++i)
    {
        std::cout << " ";
    }
}


Line::Line() noexcept
: Shape()
{
    ; // Empty on purpose.
}

Line::~Line() noexcept
{
    ; // Empty on purpose.
}


Circle::Circle() noexcept
: Shape()
{
    ; // Empty on purpose.
}

Circle::~Circle() noexcept
{
    ; // Empty on purpose.
}


Rectangle::Rectangle() noexcept
: Shape()
{
    ; // Empty on purpose.
}

Rectangle::~Rectangle() noexcept
{
    ; // Empty on purpose.
}


Square::Square() noexcept
: Shape()
{
    ; // Empty on purpose.
}

Square::~Square() noexcept
{
    ; // Empty on purpose.
}

void PrintShapes(const std::list<Shape *>& shapesList)
{
    std::list<Shape *>::const_iterator shapesListEnd = shapesList.end();
    for(std::list<Shape *>::const_iterator iterShape = shapesList.begin(); 
        shapesListEnd != iterShape;
        ++iterShape)
    {
        (*iterShape)->Draw();
    }
}

} // namespace ilrd