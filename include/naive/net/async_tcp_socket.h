//
// Created by mqd on 2017/09/01.
//

#ifndef _NAIVE_ASYNC_TCP_SOCKET_H_
#define _NAIVE_ASYNC_TCP_SOCKET_H_

#include <string>
#include <functional>

#include "socket_addr.h"

namespace naive {

class AsyncTcpSocket
{
public:

	typedef std::function<bool()> HandleComingFunc;

	static AsyncTcpSocket* Create();

	virtual ~AsyncTcpSocket() {}

	virtual int Connect(const SocketAddr& addr) = 0;

	virtual int Listen(const SocketAddr& addr) = 0;

	virtual void Close() = 0;

	virtual int Send(const uint8_t* data, uint32_t len) = 0;

	virtual void RegisterHandleComingFunc(HandleComingFunc func) = 0;
		
	virtual int Recv(uint8_t* data, uint32_t len) = 0;

	virtual const SocketAddr& GetLocalAddr() const = 0;
		
	virtual const SocketAddr& GetRemoteAddr() const = 0;
		
protected:

	AsyncTcpSocket() {}
};
}


#endif // !_NAIVE_ASYNC_TCP_SOCKET_H_
