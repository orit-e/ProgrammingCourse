/******************************************************************************
 											
                filename:			message_q_p4.c			
                written by:     	Orit			
                reviewed by:    	Leah		
 											
 ******************************************************************************/
#include <stdio.h>      /*  sprintf(), perror() */
#include <sys/types.h>  /*  ftok(), msgget()    */
#include <sys/ipc.h>    /*  ftok(), msgget()    */
#include <sys/msg.h>    /*  msgget()            */
#include <unistd.h>     /*  sleep()             */

#define OUR_MSG_SIZE 40
#define SIZE_OF_LONG 8
#define MSG_BUFF_SIZE (OUR_MSG_SIZE + SIZE_OF_LONG)

typedef struct msg_buff
{
    long mtype;
    char mtext[OUR_MSG_SIZE];
}msg_buff_t;

enum MSG_Q_RETURN_VAL
{
    FUNC_RETURN_VAL_ERROR = -1,
    MSG_Q_SUCCESS = 0,
    MSG_Q_FTOK_ERROR,
    MSG_Q_MSGGET_ERROR
};

int main()
{
    msg_buff_t msg_ptr = {0};
    char *pathname = "./general_ping_pong_file";
    int mq_id = 0;
    int proj_id = 61;
    int status = MSG_Q_SUCCESS;
    key_t mq_key = 0;

    mq_key = ftok(pathname, proj_id);
    if(FUNC_RETURN_VAL_ERROR == mq_key)
    {
        perror("P4 ftok() failed.\n");

        return MSG_Q_FTOK_ERROR;
    }

    mq_id = msgget(mq_key, 0666);
    if(FUNC_RETURN_VAL_ERROR == mq_id)
    {
        perror("P4 msgget() failed.\n");

        return MSG_Q_MSGGET_ERROR;
    }

    sleep(5);
    while(FUNC_RETURN_VAL_ERROR != status)
    {
        status = msgrcv(mq_id, &msg_ptr, OUR_MSG_SIZE, -4l, 0);
        printf("P4 received message: %s\n", msg_ptr.mtext);
        sleep(1);
    }

    return MSG_Q_SUCCESS;
}