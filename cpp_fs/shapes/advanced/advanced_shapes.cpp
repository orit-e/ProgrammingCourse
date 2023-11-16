#include <iostream>
#include <cmath>

#include "point.hpp"
#include "advanced_Shapes.hpp"
#include "/home/user/Downloads/Ex_Shapes/Shapes/include/glut_utils.h"

namespace ilrd
{

Shape::Shape(double shapeCenterX_, double shapeCenterY_) noexcept
: m_shapeCenter(shapeCenterX_, shapeCenterY_)
{
    ; // Empty on purpose.
}

Shape::~Shape() noexcept
{
    ; // Empty on purpose.
}

//Save
void Shape::Move(double xNewLocation_, double yNewLocation_) noexcept
{
    m_shapeCenter.Move(xNewLocation_, yNewLocation_)
}

void Shape::Revolve(double pivotX_, double pivotY_, double angle_) noexcept
{
    Point tmp(pivotX_, pivotY_);

    m_shapeCenter.Revolve(&tmp, angle_);
}

const Point& Shape::GetShapeCenter() const noexcept
{
    return m_shapeCenter;
}

/******************************************************************************/
/*                                  Triangle                                  */Triangle::Triangle(double topX_, double topY_,
                    double baseRightX_, double baseRighY_,
                    double baseLeftX_, double baseLeftY_) noexcept
: Shape(InitShapeCenter(topX_, topY_, baseRightX_, baseRighY_, 
                        baseLeftX_, baseLeftY_))
, m_top(topX_, topY_)
, m_baseRight(baseRightX_, baseRighY_)
, m_baseLeft(baseLeftX_, baseLeftY_)
{
    ; // Empty on purpose.
}

Triangle::~Triangle() noexcept
{
    ; // Empty on purpose.
}

void Triangle::Draw(int color_) const noexcept
{
    DrawPolygon(color_, 3, m_top.GetX(), m_top.GetY(), 
                m_baseRight.GetX(), m_baseRight.GetY(),
                m_baseLeft.GetX(), m_baseLeft.GetY());
}

void Triangle::Rotate(double angle_) noexcept
{
    m_top.Revolve(GetShapeCenter(), angle_);
    m_baseRight.Revolve(GetShapeCenter(), angle_);
    m_baseLeft.Revolve(GetShapeCenter(), angle_);
}

//Save

void Triangle::Move(double xOffset_, double yOffset_) noexcept
{
    Point offset(xOffset_, yOffset_);

    Point centerBeforChange = GetShapeCenter() + offset;
    Shape::Move(centerBeforChange.GetX(), centerBeforChange.GetY());
    
    m_top += offset;
    m_baseRight += offset;
    m_baseLeft += offset;
}

void Triangle::Revolve(double pivotX_, double pivotY_, 
                        double angle_) noexcept
{
    Shape::Revolve(pivotX_, pivotY_, angle_);
    
    Point pivot(pivotX_, pivotY_);
    m_top.Revolve(pivot, angle_);
    m_baseRight.Revolve(pivot, angle_);
    m_baseLeft.Revolve(pivot, angle_);
}

Point Triangle::InitShapeCenter(double topX_, double topY_,
                                double baseRightX_, double baseRightY_,
                                double baseLeftX_, double baseLeftY_)
{
    double topSquare = pow(topX_, 2) + pow(topY_, 2);
    double baseRightSquare = pow(baseRightX_, 2) + pow(baseRightY_, 2);
    double baseLeftSquare = pow(baseLeftX_, 2) + pow(baseLeftY_, 2);

    double sX = (topSquare * (baseRightY_ - baseLeftY_) +
                    topY_ * (baseLeftSquare - baseRightSquare) + 
                    baseRightSquare * baseLeftY_ - 
                    baseLeftSquare * baseRightY_) / 2;
    double sY = (topX_ * (baseRightSquare - baseLeftSquare) + 
                    topSquare * (baseLeftX_ - baseRightX_) +
                    baseRightX_ * baseLeftSquare - 
                    baseRightSquare * baseLeftX_) / 2;
    double a = topX_ * (baseRightY_ - baseLeftY_) + 
                    topY_ * (baseLeftX_ - baseRightX_) + 
                    baseRightX_ * baseLeftY_ - baseRightY_ * baseLeftX_;
    
    return Point(sX / a, sY / a);
}


} //namespae ilrd

