/******************************************************************************
 											
				File name:			watchdog_proc.c			
				Written by:     	Orit			
				Reviewed by:    	
 											
 ******************************************************************************/
#include <signal.h> /*  sigwait(), sigemptyset(), sigaddset(), sigset_t */

#include "watchdog.h"

enum IS_WD
{
    IS_WD_FALSE,
    IS_WD_TRUE
};

extern int is_wd_proc;

int main(int argc, char *argv[])
{
    is_wd_proc = IS_WD_TRUE;
    
    WDStart(argv);

    WDStop();

    (void)argc;

    return 0;
}