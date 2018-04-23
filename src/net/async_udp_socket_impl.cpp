//
// Created by mqd on 2017/09/01.
//

#include "async_udp_socket_impl.h"
#include "naive/log.h"
#include "naive/safe_delete.h"

namespace naive {

AsyncUdpSocket* AsyncUdpSocket::Create() {
	return new AsyncUdpSocketImpl();
}

AsyncUdpSocketImpl::AsyncUdpSocketImpl() :
	_handleComingFunc(nullptr),
	_isConnected(false){

}

AsyncUdpSocketImpl::~AsyncUdpSocketImpl() {
	SafeDelete(_recvWorkThread);
}

int AsyncUdpSocketImpl::Connect(const SocketAddr& addr) {
		
	_localAddr._ip = addr._ip;
	_localAddr._port = addr._port;
	LARK_SOCKET_INIT();
	_socket = LARK_NEW_SOCKET(LARK_AF_INET, LARK_SOCK_DGRAM, LARK_UDP);
	memset(&_serverAddr, 0, sizeof(_serverAddr));
	LARK_SOCKET_ADDR_INIT(_serverAddr, LARK_AF_INET, addr._ip.c_str(), addr._port);
	int ret = LARK_CONNECT(_socket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr));
	if (0 > ret) {
		NE("AsyncUdpSocketImpl::Connect %s:%d err: %d %s", addr._ip.c_str(), addr._port, ret, strerror(errno));
		return -1;
	}
	_isConnected = true;

	_recvWorkThread = BlockSingleWorkThread::Create();
	_recvWorkThread->RegisterReadFunc(_handleComingFunc);
	_recvWorkThread->RegisterInterruptFunc([this] {
		LARK_SHUTDOWN(_socket, LARK_CLOSE_BOTH);
		LARK_CLOSE_SOCKET(_socket);
	});
	_recvWorkThread->Run();

	return 0;
}

void AsyncUdpSocketImpl::Close() {
	_isConnected = false;
	_recvWorkThread->Stop();
}
	
int AsyncUdpSocketImpl::Send(const uint8_t* data, uint32_t len) {
	if (_isConnected) {
		std::lock_guard<std::mutex> lck(_sendMutex);
		LARK_SEND(_socket, (char*)data, len, 0);
		return 0;
	}
	return -1;
}

int AsyncUdpSocketImpl::RegisterHandleComingFunc(HandleComingFunc func) {
	_handleComingFunc = func;
	return 0;
}

int AsyncUdpSocketImpl::Recv(uint8_t* data, uint32_t len) {
	if (!_isConnected) {
		return -1;
	}
	return LARK_RECV(_socket, (char*)data, len, 0);
}

int AsyncUdpSocketImpl::RecvFrom(uint8_t* data, uint32_t len, SocketAddr& addr) {
	if (!_isConnected) {
		return -1;
	}
	return LARK_RECV(_socket, (char*)data, len, 0);
}

int AsyncUdpSocketImpl::SendTo(const uint8_t* data, uint32_t len, const SocketAddr& addr) {
	return 0;
}

const SocketAddr& AsyncUdpSocketImpl::GetLocalAddr() const {
	return _localAddr;
}

const SocketAddr& AsyncUdpSocketImpl::GetRemoteAddr() const {
	return _localAddr;
}
}