//
// Created by mqd on 2017/09/01.
//
#ifndef _NAIVE_COMMON_SOCKET_DEFINE_H_
#define _NAIVE_COMMON_SOCKET_DEFINE_H_

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

namespace naive{

#define LARK_AF_INET		AF_INET

#define LARK_NEW_SOCKET(AF, TYPE, PROTOCOL) 	socket(AF, TYPE, PROTOCOL); 
#define LARK_SOCK_STREAM 	SOCK_STREAM
#define LARK_SOCK_DGRAM 	SOCK_DGRAM

#define LARK_SOCKET_ADDR 	sockaddr_in


#define LARK_SEND(SKT, BUF, LEN, FLAGS)	send(SKT, BUF, LEN, FLAGS)
#define LARK_RECV(SKT, BUF, LEN, FLAGS)	recv(SKT, BUF, LEN, FLAGS)


#define LARK_SOCKET_ADDR_INIT(ADDR_IN, AF, ADDR, PORT)  ADDR_IN.sin_family = AF; \
														ADDR_IN.sin_addr.s_addr = inet_addr(ADDR); \
														ADDR_IN.sin_port = htons(PORT);


#define LARK_CONNECT(SKT, SKTNAME, NAME_LEN)	connect(SKT, SKTNAME, NAME_LEN)

#ifdef _WIN32
#define LARK_SOCKET_INIT()	WSADATA wsaData;\
							WSAStartup(MAKEWORD(2, 2), &wsaData)

#define LARK_TCP			IPPROTO_TCP
#define LARK_UDP			0
#define LARK_SOKET 			SOCKET
#define LARK_CLOSE_BOTH     2
#define LARK_SHUTDOWN		shutdown
#define LARK_CLOSE_SOCKET 	closesocket
	//todo:program end up with clean up;
	/*WSACleanup();\*/
#define LARK_RECV_WAIT		0
#else
#define LARK_SOCKET_INIT() 
#define LARK_SOKET int

#define LARK_TCP 0
#define LARK_UDP 0
#define LARK_SOCKET_ADDR sockaddr_in
#define LARK_CLOSE_BOTH     SHUT_RDWR
#define LARK_SHUTDOWN		shutdown
#define LARK_CLOSE_SOCKET	close
#define LARK_RECV_WAIT MSG_WAITALL
#endif

};

#endif	//~_NAIVE_COMMON_SOCKET_DEFINE_H_