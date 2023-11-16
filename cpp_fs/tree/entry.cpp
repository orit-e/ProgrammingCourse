/*******************************************************************************
 * 																			   *
 * 					File name:			entry.cpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/

#include "entry.hpp"

namespace ilrd
{

Entry::Entry()
{
    ; // Empty on purpose.
}

Entry::~Entry() noexcept
{
    ; // Empty on purpose.
}

void Entry::Display(size_t level_) noexcept
{
    (void)level_;
}

} // namespace ilrd