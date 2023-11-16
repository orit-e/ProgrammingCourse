/******************************************************************************
 											
                filename:			message_q_p1.c			
                written by:     	Orit			
                reviewed by:    	Leah		
 											
 ******************************************************************************/
#include <stdio.h>      /*  sprintf(), perror() */
#include <sys/types.h>  /*  ftok(), msgget()    */
#include <sys/ipc.h>    /*  ftok(), msgget()    */
#include <sys/msg.h>    /*  msgget()            */

#define OUR_MSG_SIZE 40
#define NUM_OF_MESSAGES 10

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
    msg_buff_t msg_buff[NUM_OF_MESSAGES] = {0};
    char *pathname = "./general_ping_pong_file";
    long l_i = 0;
    int mq_id = 0;
    int proj_id = 61;
    int i = 0;
    key_t mq_key = 0;

    mq_key = ftok(pathname, proj_id);
    if(FUNC_RETURN_VAL_ERROR == mq_key)
    {
        perror("P1 ftok() failed.\n");

        return MSG_Q_FTOK_ERROR;
    }

    mq_id = msgget(mq_key, 0666 | IPC_CREAT);
    if(FUNC_RETURN_VAL_ERROR == mq_id)
    {
        perror("P1 msgget() failed.\n");

        return MSG_Q_MSGGET_ERROR;
    }
    
    for(l_i = 0; NUM_OF_MESSAGES > l_i; ++l_i)
    {
        msg_buff[l_i].mtype = l_i + 1;
        sprintf(msg_buff[l_i].mtext, "This is message no. %ld", l_i + 1);
    }

    /*msgmnb = 16,384 => 4 pages of 4 KiB*/
    for(i = 0; NUM_OF_MESSAGES > i; ++i)
    {
        msgsnd(mq_id, (void *)&msg_buff[i], OUR_MSG_SIZE, 0);
    }

    return MSG_Q_SUCCESS;
}