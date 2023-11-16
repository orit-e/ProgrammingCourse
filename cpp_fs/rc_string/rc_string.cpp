/*******************************************************************************
 * 																			   *
 * 					File name:			rc_string.cpp					       *
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

// RCString - public functions:
RCString::RCString(const char *str_)
: m_strContainer(InitializeStrContainer(str_))
{
    ; // Empty on purpose.
}

RCString::~RCString() noexcept
{
    LeaveOrDestroyStrContainer();
}

RCString::RCString(const RCString& other_)
: m_strContainer(MakeShallowCopy(other_))
{
    ; // Empty on purpose.
}

RCString& RCString::operator=(const RCString& other_)
{
    StrContainer *tmp = MakeShallowCopy(other_);

    LeaveOrDestroyStrContainer();
    m_strContainer = tmp;

    return *this;
}

RCString& RCString::Concat(const RCString& other_)
{
    size_t thisStrLen = Length();
    size_t otherStrLen = other_.Length() + NULL_CHAR;

    StrContainer *tmp = AllocForDeepCopy(thisStrLen + otherStrLen);

    memcpy(tmp->m_str, ToCStr(), thisStrLen);
    memcpy(&tmp->m_str[thisStrLen], other_.ToCStr(), otherStrLen);

    LeaveOrDestroyStrContainer();
    m_strContainer = tmp;

    return *this;
}

RCString::ProxyChar RCString::operator[](size_t index_)
{
    RCString::ProxyChar proxyStr(*this, index_);

    return proxyStr;
}

// RCString - private functions
RCString::StrContainer *RCString::InitializeStrContainer(const char *str_)
{
    size_t strLen = strlen(str_) + NULL_CHAR;
	StrContainer *tmp = AllocForDeepCopy(strLen);

    memcpy(tmp->m_str, str_, strLen);

	return tmp;
}

RCString::StrContainer *RCString::AllocForDeepCopy(size_t strLen)
{
    char *strBuffer = static_cast<char *>(operator new(sizeof(char) * 
                                            (strLen + SHARE_COUNTER_SIZE)));

	StrContainer *tmpStr = new (strBuffer) StrContainer;

    tmpStr->m_shareCounter = 1;

    return tmpStr;
}

RCString::StrContainer *RCString::MakeShallowCopy(const RCString& other_)
{
    ++other_.m_strContainer->m_shareCounter;

    return other_.m_strContainer;
}

void RCString::LeaveOrDestroyStrContainer()
{
    static RCString::leaveOrDelete_t leaveOrDelete[2] = 
    {
        &RCString::DeleteDeepCopy,
        &RCString::LeaveShallowCopy
    };
    bool isShared = IsShared();   

    (this->*leaveOrDelete[isShared])();
}

void RCString::DeleteDeepCopy()
{
    operator delete(m_strContainer);
    m_strContainer = 0;
}

void RCString::LeaveShallowCopy()
{
    --m_strContainer->m_shareCounter;
}

// streams for RCString:
std::ostream& operator<<(std::ostream& os_, const RCString& str_)
{
    return os_ << str_.ToCStr();
}

std::istream& operator>>(std::istream& is_, RCString& str_)
{
    char buffer[BUFFER_SIZE] = {0};

	is_ >> std::setw(BUFFER_SIZE) >> buffer;

	RCString tmp = buffer;

	str_ = tmp;

	return is_;
}

} // namespace ilrd