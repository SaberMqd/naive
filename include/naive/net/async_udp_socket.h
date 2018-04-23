//
// Created by mqd on 2017/09/01.
//

#ifndef _NAIVE_ASYNC_UDP_SOCKET_H_
#define _NAIVE_ASYNC_UDP_SOCKET_H_

#include <functional>
#include <cstdint>
#include <string>

#include "socket_addr.h"

namespace naive {

class AsyncUdpSocket
{
public:

	typedef std::function<bool()> HandleComingFunc;

	static AsyncUdpSocket* Create();

	virtual ~AsyncUdpSocket() {}

	virtual int Connect(const SocketAddr& addr) = 0;

	virtual void Close() = 0;

	virtual int Send(const uint8_t* data, uint32_t len) = 0;

	virtual int SendTo(const uint8_t* data, uint32_t len,const SocketAddr& addr) = 0;

	virtual int RegisterHandleComingFunc(HandleComingFunc func) = 0;

	virtual int Recv(uint8_t* data,uint32_t len) = 0;

	virtual int RecvFrom(uint8_t* data, uint32_t len, SocketAddr& addr) = 0;

	virtual const SocketAddr& GetLocalAddr() const = 0;

	virtual const SocketAddr& GetRemoteAddr() const = 0;

protected:

	AsyncUdpSocket() {}
};
}


#endif // !_NAIVE_ASYNC_UDP_SOCKET_H_
