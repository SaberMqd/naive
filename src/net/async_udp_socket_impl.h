//
// Created by mqd on 2017/09/01.
//

#ifndef _NAIVE_ASYNC_UDP_SOCKET_IMPL_H_
#define _NAIVE_ASYNC_UDP_SOCKET_IMPL_H_

#include "naive/net/async_udp_socket.h"

#include "naive/net/common_socket_define.h"
#include "naive/base_constructor.h"
#include "naive/work_thread/block_single_work_thread.h"
#include "naive/work_thread/single_work_thread.h"

#include <mutex>

namespace naive {

class AsyncUdpSocketImpl : public AsyncUdpSocket
{
public:

	explicit AsyncUdpSocketImpl();

	~AsyncUdpSocketImpl();

	int Connect(const SocketAddr& addr) override;

	void Close() override;

	int Send(const uint8_t* data, uint32_t len) override;

	int SendTo(const uint8_t* data, uint32_t len, const SocketAddr& addr) override;

	int RegisterHandleComingFunc(HandleComingFunc func) override;

	int Recv(uint8_t* data, uint32_t len) override;

	int RecvFrom(uint8_t* data, uint32_t len, SocketAddr& addr) override;

	const SocketAddr& GetLocalAddr() const override;

	const SocketAddr& GetRemoteAddr() const override;

private:

	LARK_SOKET 			_socket;
	LARK_SOCKET_ADDR 	_serverAddr;

	bool			    _isConnected;
	std::mutex			_sendMutex;

	HandleComingFunc	_handleComingFunc;
	SocketAddr			_localAddr;
	BlockSingleWorkThread *_recvWorkThread;
	DISALLOW_COPY_AND_ASSIGN(AsyncUdpSocketImpl);
};
}


#endif // !_NAIVE_ASYNC_UDP_SOCKET_IMPL_H_
