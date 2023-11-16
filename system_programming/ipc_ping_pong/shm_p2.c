/******************************************************************************
 											
                filename:			shm_p2.c			
                written by:     	Orit			
                reviewed by:    	Leah		
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf(), perror()  */
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

    shm_key = ftok(pathname, proj_id);
    if(FUNC_RETURN_VAL_ERROR == shm_key)
    {
        perror("P2 ftok() failed.\n");

        return SHM_FTOK_ERROR;
    }

    shm_id = shmget(shm_key, 1024, 0);
    if(FUNC_RETURN_VAL_ERROR == shm_id)
    {
        perror("P2 shmget() failed.\n");

        return SHM_SHMGET_ERROR;
    }

    shm_ptr = (char *)shmat(shm_id, NULL, SHM_RDONLY);
    if((char *)(FUNC_RETURN_VAL_ERROR) == shm_ptr)
    {
        perror("P2 shmat() failed.\n");

        return SHM_SHMAT_ERROR;
    }

    printf("P2 received shared memory message:\n %s\n", shm_ptr);

    return SHM_SUCCESS;
}
