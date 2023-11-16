/*******************************************************************************
 * 																			   *
 * 					File name:			entry.hpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_ENTRY_H
#define ILRD_RD9315_ENTRY_H

#include <iosfwd>

#include "utils.hpp"

namespace ilrd
{

class Entry
{
public:
    Entry();
    virtual ~Entry() noexcept;
    virtual void Display(size_t level_) noexcept =0;
private:
    Entry(const Entry& other_); // private on purpose.
    Entry& operator=(const Entry& other_); // private on purpose.
};

}

#endif // ILRD_RD9315_ENTRY_H