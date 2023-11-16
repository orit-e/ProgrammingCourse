/******************************************************************************
 											
                filename:			ext2.h			
                written by:     	Orit			
                reviewed by:    	Guy B		
 											
 ******************************************************************************/
#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /*  size_t  */

enum PRINT_FILE_AND_STAT_RETURN_VAL
{
    PRINT_FILE_AND_STAT_SUCCESS = 0,
    PRINT_FILE_AND_STAT_BAD_VDISK_PATH,
    PRINT_FILE_AND_STAT_WRONG_FILE_SYSTEM,
    PRINT_FILE_AND_STAT_COULD_NOT_FIND_FILE
};

int PrintFileAndStat(const char *vdisk_path, const char *file_path);

#endif /*__VSA_H__*/