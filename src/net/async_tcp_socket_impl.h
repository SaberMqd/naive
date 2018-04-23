//
// Created by mqd on 2017/09/01.
//

#ifndef _NAIVE_ASYNC_TCP_SOCKET_IMPL_H_
#define _NAIVE_ASYNC_TCP_SOCKET_IMPL_H_

#include "naive/net/async_tcp_socket.h"
#include "naive/net/common_socket_define.h"
#include "naive/base_constructor.h"
#include "naive/work_thread/single_work_thread.h"
#include "naive/work_thread/block_single_work_thread.h"
#include "naive/ring_buffer.h"

#include <mutex>

namespace naive {

class AsyncTcpSocketImpl : public AsyncTcpSocket
{
public:

	explicit AsyncTcpSocketImpl();
	~AsyncTcpSocketImpl();

	int Connect(const SocketAddr& addr) override;

	int Listen(const SocketAddr& addr) override;

	void Close() override;

	int Send(const uint8_t* data, uint32_t len) override;

	void RegisterHandleComingFunc(HandleComingFunc func) override;

	int Recv(uint8_t* data, uint32_t len) override;

	const SocketAddr& GetLocalAddr() const override;
		
	const SocketAddr& GetRemoteAddr() const override;

private:

	LARK_SOKET				_socket;
	LARK_SOCKET_ADDR 		_serverAddr;

	SingleWorkThread		*_sendWorkThread;
	BlockSingleWorkThread	*_recvWorkThread;

	RingBuf<uint8_t>	*_asyncBuffer;
	HandleComingFunc 	_handleComing;
		
	std::mutex			_sendMutex;
	SocketAddr			_localAddr;
	DISALLOW_COPY_AND_ASSIGN(AsyncTcpSocketImpl);
};
}


#endif // !_NAIVE_ASYNC_TCP_SOCKET_IMPL_H_
