/******************************************************************************
 											
                filename:			tcp_ping_pong_server.c			
                written by:     	Orit			
                reviewed by:    	Rotem		
 											
 ******************************************************************************/
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>      /*  printf(), perror()                  */
#include <unistd.h>     /*  close()                             */
#include <sys/socket.h> /*  accept(), inet_ntoa(), socklen_t    */
#include <netinet/in.h> /*  inet_ntoa(), struct sockaddr_in     */
#include <arpa/inet.h>  /*  inet_ntoa()                         */
#include <errno.h>      /*  errno                               */

#include "tcp_udp_ping_pong_utils.h"

int main()
{
    struct sockaddr_in server_socket_addr = {0};
    struct sockaddr_in client_socket_addr = {0};
    socklen_t sockaddr_in_size = sizeof(struct sockaddr_in);
    int server_socket_fd = 0;
    int client_socket_fd = 0;
    int status = 0;
    char server_message[] = "Pong!";

    server_socket_fd = TCPUDPSetServer(&server_socket_addr, 
                                        SOCKET_PING_PONG_TCP);
    if(-1 == server_socket_fd)
    {
        return -1;
    }

    client_socket_fd = accept(server_socket_fd, 
                                (struct sockaddr *)&client_socket_addr, 
                                &sockaddr_in_size);
    if(-1 == client_socket_fd)
    {
        perror("Error in accept()");
        close(server_socket_fd);
        
        return -1;
    }

    printf("Accepted connection to client: %s\n", 
            inet_ntoa(client_socket_addr.sin_addr));

    status = TCPPingPongLoop(client_socket_fd, server_message, 
                                TCP_UDP_PING_PONG_SERVER);
    
    close(client_socket_fd);
    close(server_socket_fd);
    
    return status;
}