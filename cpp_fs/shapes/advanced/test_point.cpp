#include <iostream>

#include "/home/user/Downloads/Ex_Shapes/Shapes/include/point.hpp"

using namespace ilrd;

int main()
{
    Point p1;
    std::cout << p1 << std::endl;

    p1.Move(2.0, 2.0);
    Point p2(1.0, 2.0);

    std::cout << (p1 + p2) << std::endl;
    std::cout << (p1 - p2) << std::endl;
    std::cout << (p1 * 2.0) << std::endl;
    std::cout << (p1 / 2.0) << std::endl;

    return 0;
}