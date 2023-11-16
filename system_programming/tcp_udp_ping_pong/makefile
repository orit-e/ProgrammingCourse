CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g

DEPS = tcp_udp_ping_pong_utils.h
OBJS = tcp_udp_ping_pong_utils.o udp_ping_pong_server.o udp_ping_pong_client.o \
		tcp_ping_pong_server.o tcp_ping_pong_client.o udp_broadcast_sender.o \
		udp_broadcast_receiver.o

UDP_SERVER_OBJS = tcp_udp_ping_pong_utils.o udp_ping_pong_server.o
UDP_CLIENT_OBJS = tcp_udp_ping_pong_utils.o udp_ping_pong_client.o

TPC_SERVER_OBJS = tcp_udp_ping_pong_utils.o tcp_ping_pong_server.o
TPC_CLIENT_OBJS = tcp_udp_ping_pong_utils.o tcp_ping_pong_client.o

BROADCAST_SENDER_OBJS = tcp_udp_ping_pong_utils.o udp_broadcast_sender.o
BROADCAST_RECEIVER_OBJS = tcp_udp_ping_pong_utils.o udp_broadcast_receiver.o

UDP_EXES = udp_ping_pong_server.out udp_ping_pong_client.out
TPC_EXES = tcp_ping_pong_server.out tcp_ping_pong_client.out
BROADCAST_EXES = udp_broadcast_sender.out udp_broadcast_receiver.out

OBJ_COMP = $(CC) $(CFLAGS) -c $< -o $@
EXE_COMP = $(CC) $(CFLAGS) -o $@ $^

.PHONY : clean udp_exe tcp_exe bro_exe

udp_exe : $(UDP_EXES)
tcp_exe : $(TPC_EXES)
bro_exe : $(BROADCAST_EXES)

%.o : %.c $(DEPS)
	$(OBJ_COMP)

udp_ping_pong_server.out : $(UDP_SERVER_OBJS)
	$(EXE_COMP)

udp_ping_pong_client.out : $(UDP_CLIENT_OBJS)
	$(EXE_COMP)

tcp_ping_pong_server.out : $(TPC_SERVER_OBJS)
	$(EXE_COMP)

tcp_ping_pong_client.out : $(TPC_CLIENT_OBJS)
	$(EXE_COMP)

udp_broadcast_sender.out : $(BROADCAST_SENDER_OBJS)
	$(EXE_COMP)

udp_broadcast_receiver.out : $(BROADCAST_RECEIVER_OBJS)
	$(EXE_COMP)

clean :
	$(RM) $(UDP_EXES) $(TPC_EXES) $(BROADCAST_EXES) $(OBJS)