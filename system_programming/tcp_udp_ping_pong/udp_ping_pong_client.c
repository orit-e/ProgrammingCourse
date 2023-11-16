/******************************************************************************
 											
                filename:			udp_ping_pong_client.c			
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
    int client_socket_fd = 0;
    int status = 0;
    char client_message[] = "Ping!";

    assert(2 == argc);

    client_socket_fd = TCPUDPSetClient(argv[1], &server_socket_addr, 
                                        SOCKET_PING_PONG_UDP);
    if(-1 == client_socket_fd)
    {
        return -1;
    }

    status = UDPPingPongLoop(client_socket_fd, client_message, 
                                &server_socket_addr, 
                                TCP_UDP_PING_PONG_CLIENT);

    close(client_socket_fd);

    return status;
}