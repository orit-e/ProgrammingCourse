#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/**
 * Defines the amount of seconds the WD tool will wait for response from the 
 * designated process. The number defined should be at least 2 seconds or more.
 * Default at 5 seconds wait for best results.
 * */
#define WD_TIMED_WAIT (5)

enum WD_START_EXIT_STATUS
{
    WD_START_SUCCESS = 0,
    WD_START_ALLOC_FAIL,
    WD_START_SEM_FAIL,
    WD_START_PTHREAD_CREATE_FAIL,
    WD_START_FORK_FAIL,
    WD_START_CHILD_PROCESS_NOT_RESPOND
};

enum WD_STOP_EXIT_STATUS
{
    WD_STOP_SUCCESS = 0,
    WD_STOP_FAIL
};

/**
 * @INTRODUCTION:
 * Watchdog is used to guard a critical section of code in a process from 
 * undesired run time mailfunctions (e.g.: untimely termination, process 
 * blockage). When a critical problem is detected, the watchdog will restart 
 * the the protected process (from the top). Each process can be protected by 
 * one wathcdog at a time.
 * The watchdog uses the following resources:
 * *A watchdog process that will run along side the user's process.
 * *In the user's process - another thread will be allocated for comunication
 *  with the watchdog process.
 * *The signal SIGUSR1 is needed for comunication between the watchdog and the 
 *  user's protected process. Thus the signal handler for this signal will be 
 *  occupied by the watchdog functions.
 * *Two named semaphores (POSIX) are occupied.
 * The functios WDStart() and WDStop() provide a user friendly interface for 
 * initializing and removing the watchdog protection.
 **/

/**
 * @DESCRIPTION:
 * WDStart() function creates and initializes the watchdog (see INTRODUCTION 
 * for details). The function needs to recieve the same argv passed to the 
 * user process' main() function. From the moment the function returns 
 * successfully, the process is protected. This protection can be undone by 
 * calling the WDStop() function.
 * 
 * @PARAM:
 * argv             type char **, the same argv passed to the user process' 
 *                  main() function.
 *
 * @RETURN:
 * WDStart() function returns 0 for success (WD_START_SUCCESS), or a non-zero 
 * value detailed in WD_START_EXIT_STATUS enumerations above indicating the 
 * cause of the failure (see ERROR for details).
 *
 * @ERROR:
 * If fails, the function will return one of the following error numbers:
 * *WD_START_ALLOC_FAIL                 -   dynamic allocation failled.
 * *WD_START_SEM_FAIL                   -   semaphore failed to be created or 
 *                                          openned.
 * *WD_START_PTHREAD_CREATE_FAIL        -   whatchdog designated thread failled 
 *                                          to be created.
 * *WD_START_FORK_FAIL                  -   failled to fork a child process.
 * *WD_START_CHILD_PROCESS_NOT_RESPOND  -   after child process forked, child 
 *                                          was not responsive.
 * 
 * @WARNING:
 * *argv must be the same argv passed to the user process' main() function. 
 *  Otherwise undefined behavior will occure.
 * *Attempting to call WDStart() while watchdog is allready running will lead 
 *  to undefined behavior.
 * *The watchdog uses SIGUSR1 signal for inter process communication. Altering 
 *  the SIGUSR1 signal handler while the watchdog is running will lead to 
 *  undefined behavior.
 * *Trying to manually communicate with the watchdog process will lead to 
 *  undefined behavior.
 * *Trying to terminate the watchdog process manually, and not with WDStop() 
 *  function, will result in undefined behavior.
 */
int WDStart(char *argv[]);

/**
 * @DESCRIPTION:
 * WDStop() function stops the running watchdog created in WDStart() (see 
 * INTRODUCTION for details), clean and frees all resources allocated by 
 * WDStart().
 *
 * @RETURN:
 * WDStop() function returns WD_STOP_SUCCESS for success, or WD_STOP_FAIL 
 * in the case of failure.
 *
 * @ERROR:
 * If fails, the function will return WD_STOP_FAIL.
 * 
 * @WARNING:
 * *Attempting to call WDStop() while watchdog is not running (no call to 
 *  WDStart() was previousely made) will lead to undefined behavior.
 * *Trying to terminate the watchdog process manually, and not with WDStop() 
 *  function, will result in undefined behavior.
 */
int WDStop(void);

#endif /* __WATCHDOG_H__ */