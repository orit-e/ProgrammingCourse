/*******************************************************************************
 * 																			   *
 * 					File name:			file.hpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_FILE_H
#define ILRD_RD9315_FILE_H

#include <iosfwd>
#include <string>

#include "entry.hpp"
#include "utils.hpp"

namespace ilrd
{
class File : public Entry
{
public:
    explicit File(const std::string& fileName_); // May throw bad_alloc.
    virtual ~File() noexcept;
    virtual void Display(size_t level_ = 0) noexcept;
private:
    std::string m_fileName;

    File(const File& other_); // private on purpose.
    File& operator=(const File& other_); // private on purpose.
};

}

#endif // ILRD_RD9315_FILE_H