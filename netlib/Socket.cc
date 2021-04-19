#include "./Socket.h"
#include "./SockFunc.h"
#include "./SockAddr.h"

#include <string.h>

using namespace netlib;


Socket::~Socket() {
    netlib::close(_sockFd);
}

void Socket::bindSockAddr(const SockAddr &localAddr) {
    /// 将地址绑定套接字
    netlib::bind(_sockFd, (const struct sockaddr*)(&localAddr.getSockAddr()));
}

void Socket::listen() {
    /// 
    netlib::listen(_sockFd);
}

int Socket::accept(SockAddr *peerAddr) {
    struct sockaddr_in addr;
    ::bzero(&addr, sizeof addr);
    int newFd = netlib::accept(_sockFd, &addr);
    if(newFd >= 0) {
        peerAddr->setSockAddrInet(addr);
    }
    return newFd;
}