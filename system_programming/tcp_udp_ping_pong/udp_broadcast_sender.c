/******************************************************************************
 											
                filename:			udp_broadcast_sender.c			
                written by:     	Orit			
                reviewed by:    	Rotem		
 											
 ******************************************************************************/
#include <unistd.h>     /*  close()             */
#include <netinet/in.h> /*  struct sockaddr_in  */
#include <assert.h>     /*  assert()            */

#include "tcp_udp_ping_pong_utils.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in server_socket_addr = {0};
    int sender_socket_fd = 0;
    int status = 0;
    char broadcast_message[] = "Hello! This is a broadcast message!";

    assert(2 == argc);

    sender_socket_fd = TCPUDPSetClient(argv[1], &server_socket_addr, 
                                        SOCKET_BROADCAST_UDP);
    if(-1 == sender_socket_fd)
    {
        return -1;
    }

    status = UPDSendBroadcast(sender_socket_fd, broadcast_message, 
                                &server_socket_addr);

    close(sender_socket_fd);

    return status;
}