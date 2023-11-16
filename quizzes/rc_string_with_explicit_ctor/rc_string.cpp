#include <iostream>
#include <cstring>

#include "rc_string.hpp"

namespace ilrd
{

String::String(const char *str_)
: m_rc(CreateDeepCopy(str_))
{
    // Empty on purpose.
}

String::~String()
{
    DecreaseCountOrDeleteRC();
}

String& String::operator=(const String& other_)
{
    String::RC *temp = other_.m_rc;
    ++temp->m_count;

    DecreaseCountOrDeleteRC();
    m_rc = temp;

    return *this;
}

String& String::operator=(const char *str_)
{
    String::RC *temp = CreateDeepCopy(str_);
    DecreaseCountOrDeleteRC();
    m_rc = temp;

    return *this;
}

void String::DecreaseCountOrDeleteRC()
{
    if(1 == m_rc->m_count)
    {
        delete(m_rc);
        m_rc = 0;
    }
    else
    {
        --m_rc->m_count;
    }
}

String::RC *String::CreateDeepCopy(const char *str_)
{
    String::RC *temp = static_cast<RC *>(operator new(sizeof(RC)));

    temp->m_str = const_cast<char *>(str_);
    temp->m_count = 1;

    return temp;
}

size_t String::GetCount()
{
    return m_rc->m_count;
}
const char *String::GetStr()
{
    return m_rc->m_str;
}

}