/*******************************************************************************
 * 																			   *
 * 					File name:			simple_string.cpp					   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy A								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream>	//	size_t, new[], delete[], ostream, istream
#include <iomanip>	//	setw()
#include <cstring>	//	strlen(), strcmp(), memcpy()

#include "simple_string.hpp"

namespace ilrd
{
String::String(const char *s_)
: m_cstr(AllocateString(s_))
{
	; // Empty on purpose.
}

String::~String()
{
	delete[] m_cstr;
	m_cstr = 0;
}

String::String(const String& other_)
: m_cstr(AllocateString(other_.CStr()))
{
	; // Empty on purpose.
}

String& String::operator=(const String& other_)
{
	char *newStr = AllocateString(other_.CStr());

	delete[] m_cstr;
	m_cstr = newStr;

	return *this;
}

size_t String::Length() const
{
	return strlen(m_cstr);
}

const char* String::CStr() const
{
	return static_cast<const char *>(m_cstr);
}

bool operator==(const String& str1_, const String& str2_)
{
	return (0 == strcmp(str1_.CStr(), str2_.CStr()));
}


bool operator>(const String& str1_, const String& str2_)
{
	return (0 < strcmp(str1_.CStr(), str2_.CStr()));
}

bool operator<(const String& str1_, const String& str2_)
{
	return (0 > strcmp(str1_.CStr(), str2_.CStr()));
}

std::ostream& operator<<(std::ostream& os_, const String& str_)
{
	return os_ << str_.CStr();
}

std::istream& operator>>(std::istream& is_, String& str_)
{
	char buffer[BUFFER_SIZE] = {0};

	is_ >> std::setw(BUFFER_SIZE) >> buffer;

	String tmpS = buffer;

	str_ = tmpS;

	return is_;
}

char *String::AllocateString(const char *s_)
{
	size_t strLen = strlen(s_) + NULL_CHAR;
	char *tmp = new char[strLen];
	memcpy(tmp, s_, strLen);

	return tmp;
}

} // namespace ilrd