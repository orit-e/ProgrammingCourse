/******************************************************************************
 											
                filename:			ext2.c			
                written by:     	Orit			
                reviewed by:    			
 											
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "ext2.h"

void CreateEverything();

int main()
{
    /*CreateEverything();*/
    /*PrintFileAndStat("/dev/ram0", "/file1.txt");*/
    /*PrintFileAndStat("/dev/ram0", "/dir1/file2.txt");*/
    PrintFileAndStat("/dev/ram0", "/file3.txt");
    /*PrintFileAndStat("/dev/ram0", "/file4.txt");*/

    return 0;
}

void CreateEverything()
{
    system("./create_script_for_ext2_ex");
}
