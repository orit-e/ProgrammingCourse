/******************************************************************************
 											
                filename:			udp_broadcast_receiver.c			
                written by:     	Orit			
                reviewed by:    	Rotem		
 											
 ******************************************************************************/
#include <unistd.h>     /*  close()             */
#include <netinet/in.h> /*  struct sockaddr_in  */

#include "tcp_udp_ping_pong_utils.h"

int main()
{
    struct sockaddr_in server_socket_addr = {0};
    struct sockaddr_in client_socket_addr = {0};
    int status = 0;
    int receiver_socket_fd = 0;

    receiver_socket_fd = TCPUDPSetServer(&server_socket_addr, 
                                        SOCKET_PING_PONG_UDP);
    if(-1 == receiver_socket_fd)
    {
        return -1;
    }

    status = UPDRecvBroadcast(receiver_socket_fd, &client_socket_addr);
    
    close(receiver_socket_fd);

    return status;
}