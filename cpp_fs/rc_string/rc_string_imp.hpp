/*******************************************************************************
 * 																			   *
 * 					File name:			rc_string_imp.hpp				       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B  								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream> //
#include <cstring>	//	strlen(), strcmp()

#include "rc_string.hpp"

namespace ilrd
{

inline const char *RCString::ToCStr() const noexcept
{
    return m_strContainer->m_str;
}

inline size_t RCString::Length() const noexcept
{
    return strlen(ToCStr());
}

inline bool RCString::IsShared() const noexcept
{
    return (1 < m_strContainer->m_shareCounter);
}

inline char RCString::operator[](size_t index_) const
{
    return ToCStr()[index_];
}

inline bool operator==(const RCString& lhs_, const RCString& rhs_)
{
    return (0 == strcmp(lhs_.ToCStr(), rhs_.ToCStr()));
}

inline bool operator!=(const RCString& lhs_, const RCString& rhs_)
{
    return !(lhs_ == rhs_);
}

inline bool operator>(const RCString& lhs_, const RCString& rhs_)
{
    return (0 < strcmp(lhs_.ToCStr(), rhs_.ToCStr()));
}

inline bool operator<(const RCString& lhs_, const RCString& rhs_)
{
    return (0 > strcmp(lhs_.ToCStr(), rhs_.ToCStr()));
}

} // namespace ilrd