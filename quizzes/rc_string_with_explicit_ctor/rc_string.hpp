#ifndef ILRD_RD9315_RC_STRING_HPP
#define ILRD_RD9315_RC_STRING_HPP

#include <iosfwd> // size_t

#include "../../cpp_fs/utils/utils.hpp"

namespace ilrd
{
class String
{
public:
    explicit String(const char *str_ = "");
    ~String();
    String& operator=(const String& other_);
    String& operator=(const char *str_);
    size_t GetCount(); // For tests
    const char *GetStr(); // For tests
private:
    struct RC
    {
        char *m_str;
        size_t m_count;
    };
    RC *m_rc;

    void DecreaseCountOrDeleteRC();
    RC *CreateDeepCopy(const char *str_);
};
} // namespace ilrd
#endif // ILRD_RD9315_RC_STRING_HPP