/******************************************************************************
 											
                filename:			tcp_ping_pong_client.c			
                written by:     	Orit			
                reviewed by:    	Rotem		
 											
 ******************************************************************************/
#include <stdio.h>      /*  printf(), perror()      */
#include <unistd.h>     /*  close()                 */
#include <netinet/in.h> /*  struct sockaddr_in      */
#include <sys/socket.h> /*  connect(), socklen_t    */
#include <assert.h>     /*  assert()                */
#include <errno.h>      /*  errno                   */

#include "tcp_udp_ping_pong_utils.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in server_socket_addr = {0};
    socklen_t server_socket_addr_len = sizeof(struct sockaddr_in);
    int client_socket_fd = 0;
    int status = 0;
    char client_message[] = "Ping!";

    assert(2 == argc);

    client_socket_fd = TCPUDPSetClient(argv[1], &server_socket_addr, 
                                        SOCKET_PING_PONG_TCP);
    if(-1 == client_socket_fd)
    {
        return -1;
    }

    status = connect(client_socket_fd, (struct sockaddr *)&server_socket_addr, 
                        server_socket_addr_len);
    if(-1 == status)
    {
        perror("Error in connect()");
        close(client_socket_fd);

        return -1;
    }

    printf("Achieved connection to server.\n");

    status = TCPPingPongLoop(client_socket_fd, client_message, 
                                TCP_UDP_PING_PONG_CLIENT);

    close(client_socket_fd);

    return 0;
}