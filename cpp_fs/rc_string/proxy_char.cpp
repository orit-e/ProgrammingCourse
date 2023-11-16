/*******************************************************************************
 * 																			   *
 * 					File name:			proxy_char.cpp					       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B  								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream> //
#include <iomanip>	//	setw()
#include <cstring>	//	strlen(), memcpy()

#include "rc_string.hpp"

namespace ilrd
{

RCString::ProxyChar::ProxyChar(RCString& str_, size_t index_) noexcept
: m_rCStr(str_)
, m_index(index_)
{
    ; // Empty on purpose.
}

RCString::ProxyChar::~ProxyChar() noexcept
{
    ; // Empty on purpose.
}

RCString::ProxyChar::ProxyChar(const RCString::ProxyChar& other_) noexcept
: m_rCStr(other_.m_rCStr)
, m_index(other_.m_index)
{
    ; // Empty on purpose.
}

RCString::ProxyChar& RCString::ProxyChar::operator=(const char& c_)
{
    static RCString::ProxyChar::isMakeDeepCopy_t isMakeDeepCopy[2] =
    {
        &RCString::ProxyChar::DoNothing,
        &RCString::ProxyChar::ProxyMakeDeepCopy
    };
    bool isShared = m_rCStr.IsShared();

    (this->*isMakeDeepCopy[isShared])();
    
    m_rCStr.m_strContainer->m_str[m_index] = c_;

    return *this;
}

RCString::ProxyChar::operator char() const noexcept
{
    return m_rCStr.m_strContainer->m_str[m_index];
}

void RCString::ProxyChar::ProxyMakeDeepCopy()
{
    RCString::StrContainer *tmp = 
                            m_rCStr.InitializeStrContainer(m_rCStr.ToCStr());

    m_rCStr.LeaveOrDestroyStrContainer();
    m_rCStr.m_strContainer = tmp;
}

void RCString::ProxyChar::DoNothing()
{
    ; // Empty on purpose.
}

} // namespace ilrd