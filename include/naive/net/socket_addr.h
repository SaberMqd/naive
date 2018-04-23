#ifndef _NAIVE_SOCKET_ADDR_H_
#define _NAIVE_SOCKET_ADDR_H_

#include <string>

namespace naive {

struct SocketAddr{
	std::string _ip = "";
	uint16_t _port = 0;
};
}

#endif // !_NAIVE_SOCKET_ADDR_H_
