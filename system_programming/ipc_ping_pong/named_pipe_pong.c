/******************************************************************************
 											
                filename:			named_pipe_pong.c			
                written by:     	Orit			
                reviewed by:    	Leah		
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf(), perror()                  */
#include <unistd.h>     /*  write(), read(), close(), unlink()  */
#include <sys/types.h>  /*  mkfifo(), open()                    */
#include <sys/stat.h>   /*  mkfifo(), open()                    */
#include <fcntl.h>      /*  open()                              */

#define NUM_OF_LOOPS 5
#define SIZE_OF_WRITE 6
#define READ_BUFF_SIZE 1024

enum NAMED_PIPE_PONG_RETURN_VAL
{
    FUNC_RETURN_VAL_ERROR = -1,
    NAMED_PIPE_PONG_SUCCESS = 0,
    NAMED_PIPE_PONG_OPEN_ERROR
};

int main()
{
    const char *fifo_file_path = "/tmp/fifofile";
    int fifo_fd = 0;
    int status = NAMED_PIPE_PONG_SUCCESS;
    int i = 0;
    char buff_read[1024] = {0};
    char buff_write[] = "Pong!";

    for(i = 0; NUM_OF_LOOPS > i && 0 == status; ++i)
    {
        fifo_fd = open(fifo_file_path, O_RDONLY);
        if(FUNC_RETURN_VAL_ERROR == fifo_fd)
        {
            perror("Pong open() failed.\n");
            
            return NAMED_PIPE_PONG_OPEN_ERROR;
        }
        read(fifo_fd, buff_read, READ_BUFF_SIZE);
        close(fifo_fd);

        printf("%s\n", buff_read);
        
        fifo_fd = open(fifo_file_path, O_WRONLY);
        if(FUNC_RETURN_VAL_ERROR == fifo_fd)
        {
            perror("Pong open() failed.\n");
            
            return NAMED_PIPE_PONG_OPEN_ERROR;
        }
        write(fifo_fd, buff_write, SIZE_OF_WRITE);
        close(fifo_fd);
    }

    return NAMED_PIPE_PONG_SUCCESS;
}