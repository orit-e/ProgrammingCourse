/*******************************************************************************
 * 																			   *
 * 					File name:			shapes.hpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		    								   *
 * 																			   *
 * ****************************************************************************/

#ifndef ILRD_RD9315_SHAPES_HPP
#define ILRD_RD9315_SHAPES_HPP

// Standard Includes
#include <iosfwd>   // size_t

// Local Includes
#include "utils.hpp" // noexcept

namespace ilrd
{

class Shape
{
public:
    explicit Shape() noexcept;
    virtual ~Shape() noexcept;

    // Compiler generated
    // Shape(const Shape& other_);
    // Shape& operator=(const Shape& other_);

    inline void Move(std::size_t offset_) noexcept;
    virtual void Draw() const noexcept = 0;

private:
    std::size_t m_printOffset;
};

class Line : public Shape
{
public:
    explicit Line() noexcept;
    virtual ~Line() noexcept;

    // Compiler generated
    // Line(const Line& other_);
    // Line& operator=(const Line& other_);

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

void PrintShapes(const std::list<Shape*>& shapesList);


} // namespace ilrd

#include "shapes_imp.hpp"

#endif // ILRD_RD9315_SHAPES_HPP
