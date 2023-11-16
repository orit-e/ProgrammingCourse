/*******************************************************************************
 * 																			   *
 * 					File name:			directory.cpp				           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <iterator>

#include "directory.hpp"

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

Directory::Directory(const std::string& dirName_)
: m_dirName(dirName_)
, m_dirContent(InitDirContent())
{
    ; // Empty on purpose.
}

Directory::~Directory() noexcept
{
    for(std::vector<Entry *>::iterator iter = m_dirContent.begin(); 
            iter != m_dirContent.end(); ++iter)
    {
        delete *iter;
    }
}

void Directory::Display(size_t level_) noexcept
{
    size_t dirNameStartIndex = m_dirName.rfind("/");
    std::string onlyDirName = m_dirName.substr(dirNameStartIndex + 1);

    for(size_t i = 0; i < level_; ++i)
    {
        std::cout << "\t";
    }
    std::cout << "\033[" << GREEN_COLOR << "m" 
              << onlyDirName 
              << "\033[" << RESET_STYLE << "m"
              << std::endl;
    

    ++level_;    
    for(std::vector<Entry *>::iterator iter = m_dirContent.begin(); 
            iter != m_dirContent.end(); ++iter)
    {
        (*iter)->Display(level_);
    }
}

std::string& Directory::GetName() noexcept
{
    return m_dirName;
}

std::vector<Entry *> Directory::InitDirContent()
{
    std::vector<Entry *> tempVec;

    return tempVec;
}

void Directory::AddEntry(Entry *subEntry_)
{
    m_dirContent.push_back(subEntry_);
}

} // namespace ilrd
