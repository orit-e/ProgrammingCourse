/*******************************************************************************
 * 																			   *
 * 					File name:			tree.cpp					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include "tree.hpp"
#include "entry.hpp"
#include "file.hpp"
#include "directory.hpp"

namespace ilrd
{
static void CreateTree(Directory *dirRoot_);
static void CreateSubDirectory(Directory *dirRoot_, const char *dirName_);
static void CreateSubFile(Directory *dirRoot_, const char *fileName_);

void PrintTree(std::string rootDir_)
{
    Directory dirRoot(rootDir_);

    CreateTree(&dirRoot);

    dirRoot.Display();
}


static void CreateTree(Directory *dirRoot_)
{
    DIR *dirPtr = opendir((dirRoot_->GetName()).c_str());
    if(0 == dirPtr)
    {
        std::cerr << "File " << dirRoot_->GetName() << " will not open!" 
                  << std::endl;
        // exception?
    }
    
    for(struct dirent *dirEnt = readdir(dirPtr); 
            0 != dirEnt && EBADF != errno; 
            dirEnt = readdir(dirPtr))
    {
        if('.' != *dirEnt->d_name && DT_REG == dirEnt->d_type)
        {
            CreateSubFile(dirRoot_, dirEnt->d_name);
        }
        else if('.' != *dirEnt->d_name && DT_DIR == dirEnt->d_type)
        {
            CreateSubDirectory(dirRoot_, dirEnt->d_name);
        }
    }
    closedir(dirPtr);
}

static void CreateSubDirectory(Directory *dirRoot_, const char *dirName_)
{
    std::string tempStr = dirRoot_->GetName() + "/" + dirName_;
    Directory *tempDir = new Directory(tempStr);

    dirRoot_->AddEntry(tempDir);
    CreateTree(tempDir);
}


static void CreateSubFile(Directory *dirRoot_, const char *fileName_)
{
    std::string tempStr = fileName_;
    File *tempFile = new File(tempStr);

    dirRoot_->AddEntry(tempFile);
}

}