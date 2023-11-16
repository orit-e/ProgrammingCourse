/******************************************************************************
 											
                filename:			shm_p1.c			
                written by:     	Orit			
                reviewed by:    	Leah		
 											
 ******************************************************************************/
#include <stdio.h>      /*  sprintf(), perror() */
#include <sys/types.h>  /*  ftok(), shmat()     */
#include <sys/ipc.h>    /*  ftok(), shmget()    */
#include <sys/shm.h>    /*  shnget(), shmat()   */

enum SHM_RETURN_VAL
{
    FUNC_RETURN_VAL_ERROR = -1,
    SHM_SUCCESS = 0,
    SHM_FTOK_ERROR,
    SHM_SHMGET_ERROR,
    SHM_SHMAT_ERROR
};

int main()
{
    char *pathname = "./general_ping_pong_file";
    char *shm_ptr = NULL;
    int shm_id = 0;
    int proj_id = 67;
    key_t shm_key = 0;
    char shm_message_buff[] = "Hello! this is a shared memory message!";

    shm_key = ftok(pathname, proj_id);
    if(FUNC_RETURN_VAL_ERROR == shm_key)
    {
        perror("P1 ftok() failed.\n");

        return SHM_FTOK_ERROR;
    }

    shm_id = shmget(shm_key, 1024, 0666 | IPC_CREAT | IPC_EXCL);
    if(FUNC_RETURN_VAL_ERROR == shm_id)
    {
        perror("P1 shmget() failed.\n");

        return SHM_SHMGET_ERROR;
    }

    shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if((char *)(FUNC_RETURN_VAL_ERROR) == shm_ptr)
    {
        perror("P1 shmat() failed.\n");

        return SHM_SHMAT_ERROR;
    }

    sprintf(shm_ptr, "%s", shm_message_buff);

    return SHM_SUCCESS;
}
