#ifndef ILRD_RD9315_SIMPLE_STRING_HPP
#define ILRD_RD9315_SIMPLE_STRING_HPP

/*******************************************************************************
 * 																			   *
 * 					File name:			simple_string.hpp					   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy A								   *
 * 																			   *
 * ****************************************************************************/
#include <iosfwd>

namespace ilrd
{

class String
{
public:
    String(const char *s_ = ""); // non-explicit on purpose
    ~String();
    String(const String& other_);
    String& operator=(const String& other_);

    size_t Length() const;
    const char *CStr() const;

private:
    char *m_cstr;

    static const size_t NULL_CHAR = 1;
    char *AllocateString(const char *s_);
};

static const size_t BUFFER_SIZE = 256;

// advanced
bool operator==(const String& str1_, const String& str2_);
bool operator>(const String& str1_, const String& str2_);
bool operator<(const String& str1_, const String& str2_);

// more advanced
std::ostream& operator<<(std::ostream& os_, const String& s_);
std::istream& operator>>(std::istream& is_,  String& s_);

} // namespace ilrd

#endif // ILRD_RD9315_SIMPLE_STRING_HPP