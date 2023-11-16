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

using namespace ilrd;

void TestShapes();
void TestPrintShapes();

int main()
{
    TestShapes();
    TestPrintShapes();

    return 0;
}

void TestShapes()
{
    std::cout << "TestShapes:" << std::endl;
    Line l1;
    l1.Draw();
    l1.Move(4);
    l1.Draw();

    Line l2 = l1;
    l2.Draw();

    Line l3(l1);
    l3.Draw();
}

void TestPrintShapes()
{
    std::cout << std::endl << "TestPrintShapes:" << std::endl;

    std::cout << "List in order:" << std::endl;    
    
    Line l1;
    l1.Draw();
    
    Circle c1;
    c1.Draw();
    
    Rectangle r1;
    r1.Draw();
    
    Square s1;
    s1.Draw();


    Line l2;
    l2.Move(1);
    l2.Draw();

    Circle c2;
    c2.Move(1);
    c2.Draw();

    Rectangle r2;
    r2.Move(1);
    r2.Draw();

    Square s2;
    s2.Move(1);
    s2.Draw();


    Line l3;
    l3.Move(2);
    l3.Draw();

    Circle c3;
    c3.Move(2);
    c3.Draw();

    Rectangle r3;
    r3.Move(2);
    r3.Draw();

    Square s3;
    s3.Move(2);
    s3.Draw();

    std::list<Shape*> shapesList;

    shapesList.push_back(&l1);
    shapesList.push_back(&c1);
    shapesList.push_back(&r1);
    shapesList.push_back(&s1);
    shapesList.push_back(&l2);
    shapesList.push_back(&c2);
    shapesList.push_back(&r2);
    shapesList.push_back(&s2);
    shapesList.push_back(&l3);
    shapesList.push_back(&c3);
    shapesList.push_back(&r3);
    shapesList.push_back(&s3);

    std::cout << std::endl << "Calling PrintShapes:" << std::endl;

    PrintShapes(shapesList);
}