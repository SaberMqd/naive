//
// Created by mqd on 2017/09/01.
//

#include "async_tcp_socket_impl.h"
#include "naive/log.h"
#include <regex>

namespace naive {

AsyncTcpSocket* AsyncTcpSocket::Create() {
	return new AsyncTcpSocketImpl();
}

AsyncTcpSocketImpl::AsyncTcpSocketImpl() :
	_socket(0),
	_asyncBuffer(nullptr),
	_sendWorkThread(nullptr),
	_recvWorkThread(nullptr) {
}

AsyncTcpSocketImpl::~AsyncTcpSocketImpl() {
	Close();
}

int AsyncTcpSocketImpl::Connect(const SocketAddr& addr) {

	_asyncBuffer = new RingBuf<uint8_t>(1024 * 1024);
	LARK_SOCKET_INIT();
	_socket = LARK_NEW_SOCKET(LARK_AF_INET, LARK_SOCK_STREAM, LARK_TCP);
	memset(&_serverAddr, 0, sizeof(_serverAddr));
	std::regex reg("^[0-9.]+$");
	std::smatch IP;
	struct hostent* phost = NULL;

	if (!std::regex_match(addr._ip, IP, reg)) {
		phost = gethostbyname(addr._ip.c_str());
		if (!phost) {
			ND("dns parse error");
			return -1;
		}
		LARK_SOCKET_ADDR_INIT(_serverAddr, LARK_AF_INET, inet_ntoa(*(struct in_addr *)phost->h_addr_list[0]), addr._port);
	}
	else {
		LARK_SOCKET_ADDR_INIT(_serverAddr, LARK_AF_INET, addr._ip.c_str(), addr._port);
	}
	int ret = LARK_CONNECT(_socket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr));
	if (ret < 0) {
	#ifdef _WIN32
		ND("_socket connect %s:%d error:%s ret:%d error%d", addr._ip.c_str(), addr._port, strerror(errno), ret, WSAGetLastError());
	#endif
		return -1;
	}

	_sendWorkThread = SingleWorkThread::Create();
	_sendWorkThread->Run([this] {
		uint8_t buf[1024]{ 0 };
		int len = _asyncBuffer->TryPop(buf, 1024);
		if (len > 0) {
			int ret = LARK_SEND(_socket, (char*)buf, len, 0);
		}
		return true;
	});

	_recvWorkThread = BlockSingleWorkThread::Create();
	_recvWorkThread->RegisterInterruptFunc([this] {
		LARK_SHUTDOWN(_socket, LARK_CLOSE_BOTH);
		LARK_CLOSE_SOCKET(_socket);
	});
	_recvWorkThread->RegisterReadFunc(_handleComing);
	_recvWorkThread->Run();
	return 0;
}

void AsyncTcpSocketImpl::Close() {
	SafeDelete(_sendWorkThread);
	SafeDelete(_recvWorkThread);
	SafeDelete(_asyncBuffer);
}

int AsyncTcpSocketImpl::Send(const uint8_t* data, uint32_t len) {
	std::lock_guard<std::mutex> lck(_sendMutex);
	if (!_asyncBuffer->Push(data, len)) {
		ND("buffer is overWrite");
		return -1;
	}
	_sendWorkThread->Notify();
	return 0;
}

void AsyncTcpSocketImpl::RegisterHandleComingFunc(HandleComingFunc func) {
	_handleComing = func;
}

int AsyncTcpSocketImpl::Recv(uint8_t* data, uint32_t len) {
	return LARK_RECV(_socket, (char*)data, len, LARK_RECV_WAIT);
}

int AsyncTcpSocketImpl::Listen(const SocketAddr& addr) {
	return 0;
}
const SocketAddr& AsyncTcpSocketImpl::GetLocalAddr() const {
	return _localAddr;
}

const SocketAddr& AsyncTcpSocketImpl::GetRemoteAddr() const {
	return _localAddr;
}

}