#include <iostream>

#include "generic_stack.hpp"

using namespace ilrd;

int main()
{
    Stack s1;
    std::cout << "Stack size: " << s1.GetSize() << std::endl;
    std::cout << "Stack capacity: " << s1.GetCapacity() << std::endl;

    int a = 5;
    void *data1 = &a;
    s1.Push(data1);
    std::cout << "Stack Peek: " << s1.Peek() << std::endl;

    std::cout << "Second stack:" << std::endl;
    Stack s2(s1);
    std::cout << "Stack Pop: " << s2.Pop() << std::endl;

    std::cout << "Third Stack:" << std::endl;
    Stack s3 = s1;
    std::cout << "Stack size: " << s3.GetSize() << std::endl;
    std::cout << "Stack capacity: " << s3.GetCapacity() << std::endl;

    return 0;
}