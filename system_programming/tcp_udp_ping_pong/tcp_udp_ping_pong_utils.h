/******************************************************************************
 											
                filename:			tcp_udp_ping_pong_utils.h			
                written by:     	Orit			
                reviewed by:    	Rotem		
 											
 ******************************************************************************/
#ifndef __TCP_UDP_PING_PONG_UTILES_H__
#define __TCP_UDP_PING_PONG_UTILES_H__

#define MY_PORT 4950
#define MAX_BUF_LEN 100
#define NUM_OF_LOOPS 5
#define BACK_LOG 10

typedef enum SOCKET_PING_PONG_COMMUNICATION_STYLE
{
    SOCKET_PING_PONG_UDP,
    SOCKET_PING_PONG_TCP,
    SOCKET_BROADCAST_UDP
}socket_ping_pong_ty;

enum TCP_UDP_PING_PONG_CLIENT_OR_SERVER
{
    TCP_UDP_PING_PONG_SERVER,
    TCP_UDP_PING_PONG_CLIENT
};


int TCPUDPSetServer(struct sockaddr_in *server_sockaddr_in, 
                    socket_ping_pong_ty communication_style);
int TCPUDPSetClient(const char *server_service, 
                    struct sockaddr_in *server_sockaddr_in,
                    socket_ping_pong_ty communication_style);

int UDPPingPongLoop(int this_socket_fd, char *message_to_send, 
                    struct sockaddr_in *other_socket_addr, 
                    int client_or_server);
int TCPPingPongLoop(int client_socket_fd, char *message_to_send, 
                    int client_or_server);

int UPDSendBroadcast(int sender_socket_fd, char *message_to_send, 
                    struct sockaddr_in *broadcast_socket_addr);
int UPDRecvBroadcast(int receiver_socket_fd, 
                    struct sockaddr_in *broadcast_socket_addr);

#endif /* __TCP_UDP_PING_PONG_UTILES_H__ */