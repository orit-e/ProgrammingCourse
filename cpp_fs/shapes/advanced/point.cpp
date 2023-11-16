/*******************************************************************************
 * 																			   *
 * 					File name:			ponit.cpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		    								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream>
#include <cmath>

#include "point.hpp"

namespace ilrd
{

Point::Point()
: m_x(0)
, m_y(0)
{
    ; // Empty on purpose.
}

Point::Point(double x, double y)
: m_x(x)
, m_y(y)
{
    ; // Empty on purpose.
}

Point& Point::Move(double xNewLocation, double yNewLocation)
{
    m_x += xNewLocation;
    m_y += yNewLocation;
}

Point& Point::Revolve(const Point& pivot, double angle)
{
    double radius = sqrt(pow((m_x - pivot.GetX()), 2) + 
                    pow((m_y - pivot.GetY()), 2));

    this->Move(pivot.GetX() + radius * cos(angle), 
                pivot.GetY() + radius * sin(angle));
    
}

Point& Point::operator+=(const Point& other)
{
    m_x += other.m_x;
    m_y += other.m_y;
}

Point& Point::operator-=(const Point& other)
{
    m_x -= other.m_x;
    m_y -= other.m_y;
}

Point& Point::operator/=(double factor)
{
    m_x /= factor;
    m_y /= factor;
}

Point& Point::operator*=(double factor)
{
    m_x *= factor;
    m_y *= factor;
}

Point Point::operator+(const Point& point1, const Point& point2)
{
    Point tmp(point1);
    
    tmp += point2;

    return tmp;
}

Point Point::operator-(const Point& point1, const Point& point2)
{
    Point tmp(point1);
    
    tmp -= point2;

    return tmp;
}

Point Point::operator*(const Point& point1, double factor)
{
    Point tmp(point1);
    
    tmp *= factor;

    return tmp;
}

Point Point::operator/(const Point& point1, double factor)
{
    Point tmp(point1);
    
    tmp /= factor;

    return tmp;
}

bool Point::operator==(const Point& point1, const Point& point2)
{
    return (point1.GetX() == point2.GetX() && point1.GetY() == point2.GetY());
}

bool Point::operator!=(const Point& point1, const Point& point2)
{
    return !(point1 == point2);
}

std::ostream& Point::operator<<(std::ostream& os, const Point& point)
{
    return os << "(" << point.GetX() << ", " << point.GetY() << ")";
}

} //namespae ilrd
