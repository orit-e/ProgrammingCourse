/******************************************************************************
 											
                filename:			pipe_ping_pong.c			
                written by:     	Orit			
                reviewed by:    	Leah		
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf(), perror()                  */
#include <unistd.h>     /*  pipe(), write(), read(), close()    */
#include <sys/types.h>  /*  pid_t                               */
#include <sys/wait.h>   /*  wait()                              */

#define NUM_OF_LOOPS 5
#define SIZE_OF_WRITE 6
#define READ_BUFF_SIZE 1024

enum READER_WRITER_ENDS
{
    READER_PIPE_END = 0,
    WRITER_PIPE_END,
    FDS_PER_PIPE
};

enum PIPE_PING_PONG_RETURN_VAL
{
    PIPE_PING_PONG_SUCCESS = 0,
    PIPE_PING_PONG_ERROR
};

void Ping(int fd_ping_write, int fd_pong_read);
void Pong(int fd_ping_read, int fd_pong_write);

int main()
{
    int fds_ping[FDS_PER_PIPE] = {0};
    int fds_pong[FDS_PER_PIPE] = {0};
    int status = PIPE_PING_PONG_SUCCESS;
    pid_t child_pid = 0;

    status = pipe(fds_ping);
    if(PIPE_PING_PONG_SUCCESS != status)
    {
        perror("Error in first pipe() call.\n");
        
        return PIPE_PING_PONG_ERROR;
    }
    status = pipe(fds_pong);
    if(PIPE_PING_PONG_SUCCESS != status)
    {
        perror("Error in second pipe() call.\n");
        close(fds_ping[READER_PIPE_END]);
        close(fds_ping[WRITER_PIPE_END]);
        
        return PIPE_PING_PONG_ERROR;
    }

    child_pid = fork();
    if(0 == child_pid)
    {
        close(fds_ping[WRITER_PIPE_END]);
        close(fds_pong[READER_PIPE_END]);
        
        Pong(fds_ping[READER_PIPE_END], fds_pong[WRITER_PIPE_END]);

        close(fds_ping[READER_PIPE_END]);
        close(fds_pong[WRITER_PIPE_END]);
    }
    else
    {
        close(fds_ping[READER_PIPE_END]);
        close(fds_pong[WRITER_PIPE_END]);
        
        Ping(fds_ping[WRITER_PIPE_END], fds_pong[READER_PIPE_END]);

        close(fds_ping[WRITER_PIPE_END]);
        close(fds_pong[READER_PIPE_END]);

        status = wait(NULL);
        if(-1 == status)
        {
            perror("Wait() failed.\n");
        }
    }

    return PIPE_PING_PONG_SUCCESS;
}

void Ping(int fd_ping_write, int fd_pong_read)
{
    int i = 0;
    char buff_read[READ_BUFF_SIZE] = {0};
    char buff_write[] = "ping!";

    for(i = 0; NUM_OF_LOOPS > i; ++i)
    {
        write(fd_ping_write, buff_write, SIZE_OF_WRITE);
        read(fd_pong_read, buff_read, READ_BUFF_SIZE);
        printf("%s\n", buff_read);
    }
}

void Pong(int fd_ping_read, int fd_pong_write)
{
    int i = 0;
    char buff_read[READ_BUFF_SIZE] = {0};
    char buff_write[] = "pong!";

    for(i = 0; NUM_OF_LOOPS > i; ++i)
    {
        read(fd_ping_read, buff_read, READ_BUFF_SIZE);
        printf("%s\n", buff_read);
        write(fd_pong_write, buff_write, SIZE_OF_WRITE);
    }
}
