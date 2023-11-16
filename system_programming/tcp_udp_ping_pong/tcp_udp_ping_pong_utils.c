/******************************************************************************
 											
                filename:			tcp_udp_ping_pong_utils.c			
                written by:     	Orit			
                reviewed by:    	Rotem		
 											
 ******************************************************************************/
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>      /*  printf(), perror(), ssize_t                     */
#include <unistd.h>     /*  close()                                         */
#include <string.h>     /*  strlen()                                        */
#include <sys/types.h>  /*  recv(), send()                                  */
#include <sys/socket.h> /*  socket(), bind(), listen(), AF_INET, socklen_t, 
                            sendto(), recvfrom(), inet_ntoa(), recv(), 
                            send(), setsockopt(),  SOCK_DGRAM, SOCK_STREAM, 
                            SOL_SOCKET, SO_REUSEADDR, SO_BROADCAST          */
#include <netinet/in.h> /*  struct sockaddr_in, struct sockaddr, INADDR_ANY, 
                            in_addr_t, inet_ntoa()                          */
#include <netdb.h>      /*  struct addrinfo                                 */
#include <arpa/inet.h>  /*  htons(), inet_pton(), inet_ntoa()               */
#include <assert.h>     /*  assert()                                        */
#include <errno.h>      /*  errno                                           */

#include "tcp_udp_ping_pong_utils.h"

typedef int (*set_server_socket_t)(int);
typedef int (*upd_loop_action_t)(int, char *, size_t , struct sockaddr_in *);
typedef int (*tcp_loop_action_t)(int, char *);

static void SetServerSockAdrrIn(in_addr_t server_addr, 
                                struct sockaddr_in *server_sockaddr_in);
static int UDPRecvFromEnvelop(int this_socket_fd, char *message_to_send, 
                                size_t message_len,
                                struct sockaddr_in *other_socket_addr);
static int UDPSendToEnvelop(int this_socket_fd, char *message_to_send, 
                            size_t message_len,
                            struct sockaddr_in *other_socket_addr);
static int TCPRecvEnvelop(int client_socket_fd, char *message_to_send);
static int TCPSendEnvelop(int client_socket_fd, char *message_to_send);
static int SetSockOptTCP(int socket_fd);
static int SetSockOptUDPBroadcast(int socket_fd);
static int ListenEnvelop(int server_socket_fd);
static int SetNothing(int socket_fd);

static socklen_t socket_addr_len = sizeof(struct sockaddr); 
static int sock_type[3] = {SOCK_DGRAM, SOCK_STREAM, SOCK_DGRAM};

int TCPUDPSetServer(struct sockaddr_in *server_sockaddr_in, 
                    socket_ping_pong_ty communication_style)
{
    int server_socket_fd = 0;
    int status = 0;
    static set_server_socket_t set_socket_opt_func[2] = 
    {
        SetNothing,
        SetSockOptTCP
    };
    static set_server_socket_t set_to_listen[2] = 
    {
        SetNothing,
        ListenEnvelop
    };

    assert(NULL != server_sockaddr_in);

    server_socket_fd = socket(AF_INET, sock_type[communication_style], 0);
    if(-1 == server_socket_fd)
    {
        perror("Error in socket()");
        return -1;
    }

    status = set_socket_opt_func[communication_style](server_socket_fd);
    if(-1 == status)
    {
        perror("Error in setsockopt()");
        return -1;
    }

    SetServerSockAdrrIn(INADDR_ANY, server_sockaddr_in);

    status = bind(server_socket_fd,  (struct sockaddr *)server_sockaddr_in, 
                    socket_addr_len);
    if(-1 == status)
    {
        perror("Error in bind()");
        close(server_socket_fd);

        return -1;
    }

    status = set_to_listen[communication_style](server_socket_fd);
    if(-1 == status)
    {
        perror("Error in listen()");
        close(server_socket_fd);

        return -1;
    }

    return server_socket_fd;
}

int TCPUDPSetClient(const char *server_service, 
                    struct sockaddr_in *server_sockaddr_in,
                    socket_ping_pong_ty communication_style)
{
    static set_server_socket_t set_socket_opt_func[3] = 
    {
        SetNothing,
        SetNothing,
        SetSockOptUDPBroadcast
    };
    in_addr_t result_in_addr = 0;
    int status = 0;
    int client_socket_fd = 0;

    client_socket_fd = socket(AF_INET, sock_type[communication_style], 0);
    if(-1 == client_socket_fd)
    {
        perror("Error in socket()");
        return -1;
    }

    status = set_socket_opt_func[communication_style](client_socket_fd);
    if(-1 == status)
    {
        perror("Error in setsockopt()");
        return -1;
    }

    status = inet_pton(AF_INET, server_service, &result_in_addr);
    if(1 != status)
    {
        perror("Error in inet_pton()");
        return -1;
    }
    SetServerSockAdrrIn(result_in_addr, server_sockaddr_in);

    return client_socket_fd;
}

int UDPPingPongLoop(int this_socket_fd, char *message_to_send, 
                    struct sockaddr_in *other_socket_addr, int client_or_server)
{
    static upd_loop_action_t loop_action1[2] =
    {
        UDPRecvFromEnvelop,
        UDPSendToEnvelop
    };
    static upd_loop_action_t loop_action2[2] =
    {
        UDPSendToEnvelop,
        UDPRecvFromEnvelop
    };
    int i = 0;
    int status = 0;
    size_t message_len = 0;

    message_len = strlen(message_to_send) + 1;

    for(i = 0; i < NUM_OF_LOOPS; ++i)
    {
        status = loop_action1[client_or_server](this_socket_fd, 
                                                message_to_send, 
                                                message_len,
                                                other_socket_addr);
        if(-1 == status)
        {
            return status;
        }

        status = loop_action2[client_or_server](this_socket_fd, 
                                                message_to_send, 
                                                message_len,
                                                other_socket_addr);
        if(-1 == status)
        {
            return status;
        }
    }

    return status;
}

int TCPPingPongLoop(int client_socket_fd, char *message_to_send, 
                    int client_or_server)
{
    static tcp_loop_action_t loop_action1[2] =
    {
        TCPRecvEnvelop,
        TCPSendEnvelop
    };
    static tcp_loop_action_t loop_action2[2] =
    {
        TCPSendEnvelop,
        TCPRecvEnvelop
    };
    int i = 0;
    int status = 0;

    for(i = 0; i < NUM_OF_LOOPS; ++i)
    {
        status = loop_action1[client_or_server](client_socket_fd, 
                                                message_to_send);
        if(-1 == status)
        {
            return status;
        }

        status = loop_action2[client_or_server](client_socket_fd, 
                                                message_to_send);
        if(-1 == status)
        {
            return status;
        }
    }

    return status;
}

int UPDSendBroadcast(int sender_socket_fd, char *message_to_send, 
                    struct sockaddr_in *broadcast_socket_addr)
{
    int status = 0;
    size_t message_len = 0;

    message_len = strlen(message_to_send) + 1;
    status = UDPSendToEnvelop(sender_socket_fd, message_to_send, message_len, 
                        broadcast_socket_addr);
    
    return status;
}

int UPDRecvBroadcast(int receiver_socket_fd, 
                    struct sockaddr_in *broadcast_socket_addr)
{
    int status = 0;
    
    status = UDPRecvFromEnvelop(receiver_socket_fd, NULL, 0, 
                                broadcast_socket_addr);
    
    return status;
}

static int UDPRecvFromEnvelop(int this_socket_fd, char *message_to_send,
                            size_t message_len,
                            struct sockaddr_in *other_socket_addr)
{
    static char buffer[MAX_BUF_LEN] = {0};
    
    ssize_t received_message_len = recvfrom(this_socket_fd, buffer, 
                                        MAX_BUF_LEN - 1, 0, 
                                        (struct sockaddr *)other_socket_addr, 
                                        &socket_addr_len);
    if(-1 == received_message_len)
    {
        perror("Error in recvfrom()");

        return -1;
    }

    printf("Received a datagram from %s.\n", 
            inet_ntoa(other_socket_addr->sin_addr));
    printf("Datagram message: \"%s\"\n", buffer);

    (void)message_to_send;
    (void)message_len;
    return 0;
}

static int UDPSendToEnvelop(int this_socket_fd, char *message_to_send, 
                            size_t message_len,
                            struct sockaddr_in *other_socket_addr)
{
    ssize_t sent_message_len = sendto(this_socket_fd, message_to_send, 
                                    message_len, 0, 
                                    (struct sockaddr *)other_socket_addr, 
                                    socket_addr_len);
    if(-1 == sent_message_len)
    {
        perror("Error in sendto()");
    
        return -1;
    }

    return 0;
}

static int TCPRecvEnvelop(int client_socket_fd, char *message_to_send)
{
    static char buffer[MAX_BUF_LEN] = {0};

    ssize_t received_message_len = recv(client_socket_fd, buffer, 
                                        MAX_BUF_LEN - 1, 0);
    if(-1 == received_message_len)
    {
        perror("Error in connect()");

        return -1;
    }

    printf("Received stream message: \"%s\"\n", buffer);

    (void)message_to_send;
    return 0;
}

static int TCPSendEnvelop(int client_socket_fd, char *message_to_send)
{
    ssize_t sent_message_len = send(client_socket_fd, message_to_send, 6, 0);
    if(-1 == sent_message_len)
    {
        perror("Error in send()");

        return -1;
    }

    return 0;
}

static void SetServerSockAdrrIn(in_addr_t server_addr, 
                                struct sockaddr_in *server_sockaddr_in)
{
    server_sockaddr_in->sin_family = AF_INET;
    server_sockaddr_in->sin_port = htons(MY_PORT);
    server_sockaddr_in->sin_addr.s_addr = server_addr;
}

static int SetSockOptTCP(int socket_fd)
{
    int bool_true = 1;
    static size_t size_of_int = sizeof(int);

    return setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &bool_true, 
                        size_of_int);
}

static int SetSockOptUDPBroadcast(int socket_fd)
{
    int bool_true = 1;
    static size_t size_of_int = sizeof(int);

    return setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &bool_true, 
                        size_of_int);
}

static int ListenEnvelop(int server_socket_fd)
{
    return listen(server_socket_fd, BACK_LOG);
}

static int SetNothing(int socket_fd)
{
    (void)socket_fd;

    return 0;
}