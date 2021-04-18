#include "./Socket.h"
#include "./SockFunc.h"
#include "./SockAddr.h"

using namespace netlib;


Socket::~Socket() {
    netlib::close(_sockFd);
}

void Socket::bindSockAddr(const SockAddr &localAddr) {
    /// 将地址绑定套接字
}