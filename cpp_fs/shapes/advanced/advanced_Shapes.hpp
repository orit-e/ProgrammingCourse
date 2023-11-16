/*******************************************************************************
 * 																			   *
 * 					File name:			ponit.hpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		    								   *
 * 																			   *
 * ****************************************************************************/

#ifndef ILRD_RD9315_ADVANCED_SHAPES_HPP
#define ILRD_RD9315_ADVANCED_SHAPES_HPP

#include <iosfwd>

#include "utils.hpp"
#include "point.hpp"

namespace ilrd
{
class Shape
{
public:
    explicit Shape(double shapeCenterX_ = 0, double shapeCenterY_ = 0) noexcept;
    virtual ~Shape() noexcept;

    // Compiler generated
    // Shape(const Shape& other_);
    // Shape& operator=(const Shape& other_);

    virtual void Draw() const noexcept = 0;
    virtual void Rotate(double angle_) noexcept = 0;
    //Save
    inline void Move(double xNewLocation_, double yNewLocation_) noexcept;
    virtual void Revolve(double pivotX_, double pivotY_, 
                            double angle_) noexcept;
protected:
    inline const Point& GetShapeCenter() const noexcept;
private:
    Point m_shapeCenter;
};

class Triangle : public Shape
{
public:
    explicit Triangle(double topX_ = 0, double topY_ = 0,
                        double baseRightX_ = 0, double baseRighY_ = 0,
                        double baseLeftX_ = 0, double baseLeftY_ = 0) noexcept;
    virtual ~Triangle() noexcept;
    
    // Compiler generated
    // Circle(const Circle& other_);
    // Circle& operator=(const Circle& other_);
    
    inline virtual void Draw(int color_) const noexcept;
    virtual void Rotate(double angle_) noexcept;
    //Save
    inline void Move(double xOffset_, double yOffset_) noexcept;
    virtual void Revolve(double pivotX_, double pivotY_, 
                            double angle_) noexcept;
private:
    // Offset from m_shapeCenter (class Shape).
    Point m_top;
    Point m_baseRight;
    Point m_baseLeft;

    Point InitShapeCenter(double topX_, double topY_,
                            double baseRightX_, double baseRightY_,
                            double baseLeftX_, double baseLeftY_);
};

class Square : public Shape
{
public:
    explicit Square() noexcept;
    virtual ~Square() noexcept;

    // Compiler generated
    // Square(const Square& other_);
    // Square& operator=(const Square& other_);

    inline virtual void Draw() const noexcept;
};

class Rectangle : public Shape
{
public:
    explicit Rectangle() noexcept;
    virtual ~Rectangle() noexcept;

    // Compiler generated
    // Rectangle(const Rectangle& other_);
    // Rectangle& operator=(const Rectangle& other_);
    
    inline virtual void Draw() const noexcept;
};

class Circle : public Shape
{
public:
    explicit Circle() noexcept;
    virtual ~Circle() noexcept;
    
    // Compiler generated
    // Circle(const Circle& other_);
    // Circle& operator=(const Circle& other_);
    
    inline virtual void Draw() const noexcept;
};

class Group : public Shape
{
public:
    explicit Group() noexcept;
    virtual ~Group() noexcept;
    
    // Compiler generated
    // Circle(const Circle& other_);
    // Circle& operator=(const Circle& other_);
    
    inline virtual void Draw() const noexcept;
};

} //namespae ilrd

#endif // ILRD_RD9315_ADVANCED_SHAPES_HPP