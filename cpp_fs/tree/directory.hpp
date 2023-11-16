/*******************************************************************************
 * 																			   *
 * 					File name:			directory.hpp				           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_DIRECTORY_H
#define ILRD_RD9315_DIRECTORY_H

#include <iosfwd>
#include <string>
#include <vector>

#include "entry.hpp"
#include "utils.hpp"

namespace ilrd
{
class Directory : public Entry
{
public:
    explicit Directory(const std::string& dirName_); // May throw bad_alloc.
    virtual ~Directory() noexcept;
    virtual void Display(size_t level_ = 0) noexcept;
    void AddEntry(Entry *subEntry_); // May throw bad_alloc.
    std::string& GetName() noexcept;
private:
    std::string m_dirName;
    std::vector<Entry *> m_dirContent;

    std::vector<Entry *> InitDirContent();

    Directory(const Directory& other_); // private on purpose.
    Directory& operator=(const Directory& other_); // private on purpose.
};

}

#endif // ILRD_RD9315_DIRECTORY_H