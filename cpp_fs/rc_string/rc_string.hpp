/*******************************************************************************
 * 																			   *
 * 					File name:			rc_string.hpp					       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B  								   *
 * 																			   *
 * ****************************************************************************/

#ifndef ILRD_RD9315_RC_STRING_HPP
#define ILRD_RD9315_RC_STRING_HPP

#include <iosfwd> // size_t, ostream, istream

#include "utils.hpp"

namespace ilrd
{
class RCString
{
private:
    class ProxyChar;
public:
    RCString(const char *str_ = ""); // non-explicit on purpose, throw bad_alloc
    ~RCString() noexcept;
    RCString(const RCString& other_); // throw bad_alloc
    RCString& operator=(const RCString& other_); // throw bad_alloc

    inline const char *ToCStr() const noexcept;
    inline size_t Length() const noexcept;
    inline bool IsShared() const noexcept;
    RCString& Concat(const RCString& other_); // throw bad_alloc

    inline char operator[](size_t index_) const; // throw out_of_range
    ProxyChar operator[](size_t index_); // throw out_of_range, bad_alloc
private:
    typedef void (RCString::*leaveOrDelete_t)();

    struct StrContainer
    {
        size_t m_shareCounter;
        char m_str[1];
    };
    StrContainer *m_strContainer;

    StrContainer *InitializeStrContainer(const char *str_);
    StrContainer *AllocForDeepCopy(size_t strLen);
    StrContainer *MakeShallowCopy(const RCString& other_);
    void LeaveOrDestroyStrContainer();
    void DeleteDeepCopy();
    void LeaveShallowCopy();

    class ProxyChar
    {
    public:
        explicit ProxyChar(RCString& str_, size_t index_) noexcept;
        ~ProxyChar() noexcept;
        ProxyChar(const ProxyChar& other_) noexcept;
        ProxyChar& operator=(const char& c_); // throw bad_alloc
        
        operator char() const noexcept;
    private:
        typedef void (ProxyChar::*isMakeDeepCopy_t)();

        RCString& m_rCStr;
        size_t m_index;

        ProxyChar& operator=(const ProxyChar& other_); // Not defined
        void ProxyMakeDeepCopy();
        void DoNothing();
    }; // class ProxyChar
}; // class RCString

static const size_t SHARE_COUNTER_SIZE = sizeof(size_t);
static const size_t NULL_CHAR = sizeof(char);
static const size_t BUFFER_SIZE = 256;

// operators: 
std::istream& operator>>(std::istream& is_, RCString& str_);
std::ostream& operator<<(std::ostream& os_, const RCString& str_);

inline bool operator==(const RCString& lhs_, const RCString& rhs_);
inline bool operator!=(const RCString& lhs_, const RCString& rhs_);

inline bool operator>(const RCString& lhs_, const RCString& rhs_);
inline bool operator<(const RCString& lhs_, const RCString& rhs_);

} // namespace ilrd

#include "rc_string_imp.hpp"

#endif // ILRD_RD9315_RC_STRING_HPP
