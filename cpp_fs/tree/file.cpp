/*******************************************************************************
 * 																			   *
 * 					File name:			file.cpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream>
#include <string>

#include "file.hpp"

namespace ilrd
{

enum PRINT_COLOR
{
    RESET_STYLE = 0,
    RED_COLOR = 31,
    GREEN_COLOR,
    YELLOW_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR
};

File::File(const std::string& fileName_)
: m_fileName(fileName_)
{
    ; // Empty on purpose
}

File::~File() noexcept
{
    ; // Empty on purpose
}

void File::Display(size_t level_) noexcept
{
    for(size_t i = 0; i < level_; ++i)
    {
        std::cout << "\t";
    }
    std::cout << "\033[" << BLUE_COLOR << "m"
              << m_fileName 
              << "\033[" << RESET_STYLE << "m"
              << std::endl;
}

} // namespace ilrd