//
// Created by mqd on 2017/09/01.
//
#ifndef _NAIVE_BASE_ASYNC_SOCKET_H_
#define _NAIVE_BASE_ASYNC_SOCKET_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace naive {

#define NAIVE_AF_INET		AF_INET

#define NAIVE_NEW_SOCKET(AF, TYPE, PROTOCOL) 	socket(AF, TYPE, PROTOCOL); 
#define NAIVE_SOCK_STREAM 	SOCK_STREAM
#define NAIVE_SOCK_DGRAM 	SOCK_DGRAM

#define NAIVE_SOCKET_ADDR 	sockaddr_in

#define NAIVE_SEND(SKT, BUF, LEN, FLAGS)	send(SKT, BUF, LEN, FLAGS)
#define NAIVE_RECV(SKT, BUF, LEN, FLAGS)	recv(SKT, BUF, LEN, FLAGS)

#define NAIVE_SOCKET_ADDR_INIT(ADDR_IN, AF, ADDR, PORT)  ADDR_IN.sin_family = AF; \
														ADDR_IN.sin_addr.s_addr = inet_addr(ADDR); \
														ADDR_IN.sin_port = htons(PORT);


#define NAIVE_CONNECT(SKT, SKTNAME, NAME_LEN)	connect(SKT, SKTNAME, NAME_LEN)

#ifdef _WIN32
#define NAIVE_SOCKET_INIT()	WSADATA wsaData;\
							WSAStartup(MAKEWORD(2, 2), &wsaData)

#define NAIVE_TCP			IPPROTO_TCP
#define NAIVE_UDP			0
#define NAIVE_SOKET 			SOCKET
#define NAIVE_CLOSE_BOTH     2
#define NAIVE_SHUTDOWN		shutdown
#define NAIVE_CLOSE_SOCKET 	closesocket
	//todo:program end up with clean up;
	/*WSACleanup();\*/
#define NAIVE_RECV_WAIT		0
#else
#define NAIVE_SOCKET_INIT() 
#define NAIVE_SOKET int

#define NAIVE_TCP 0
#define NAIVE_UDP 0
#define NAIVE_SOCKET_ADDR sockaddr_in
#define NAIVE_CLOSE_BOTH     SHUT_RDWR
#define NAIVE_SHUTDOWN		shutdown
#define NAIVE_CLOSE_SOCKET	close
#define NAIVE_RECV_WAIT MSG_WAITALL
#endif

};

#endif