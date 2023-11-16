/******************************************************************************
 											
                filename:			udp_ping_pong_server.c			
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
    int server_socket_fd = 0;
    char server_msg[] = "Pong!";

    server_socket_fd = TCPUDPSetServer(&server_socket_addr, 
                                        SOCKET_PING_PONG_UDP);
    if(-1 == server_socket_fd)
    {
        return -1;
    }

    status = UDPPingPongLoop(server_socket_fd, server_msg, &client_socket_addr, 
                                TCP_UDP_PING_PONG_SERVER);
    
    close(server_socket_fd);

    return status;
}