#include <iostream>

#include "rc_string.hpp"

using namespace ilrd;

int main()
{
    const char *c1 = "hello!";
    const char *c2 = "wassup!";

    String s1(c1);
    String s2(c1);

    s2 = c2;
    std::cout << "operator=(const String& other_):" << std::endl;
    std::cout << "s2 = c2 should be m_str = wassup!, m_count = 1" << std::endl;
    std::cout << "s2: m_str = " << s2.GetStr() << ", m_count = " 
                << s2.GetCount() << std::endl;
    s2 = s1;
    std::cout << "operator=(const char *str_):" << std::endl;
    std::cout << "s2 = s1 should be m_str = hello!, m_count = 2" << std::endl;
    std::cout << "s2: m_str = " << s2.GetStr() << ", m_count = " 
                << s2.GetCount() << std::endl;

    return 0;
}