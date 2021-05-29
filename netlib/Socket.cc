#include "./Socket.h"
#include "./SockFunc.h"
#include "./SockAddr.h"
#include "./Logging.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace netlib;


Socket::~Socket() {
    netlib::close(_sockFd);
}

void Socket::bindSockAddr(const SockAddr &localAddr) {
    /// 将地址绑定套接字
    netlib::bind(_sockFd, (const struct sockaddr*)(localAddr.getSockAddr()));
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

void Socket::shutdownWrite() {
    /// 关闭写端，只能接收数据，不能发送数据
    netlib::shutdownWrite(_sockFd);
}

void Socket::setKeepAlive(bool on) {
    int val = on ? 1 : 0;
    ::setsockopt(_sockFd, SOL_SOCKET, SO_KEEPALIVE, &val, static_cast<socklen_t>(sizeof val));
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    int ret =  ::setsockopt(_sockFd, SOL_SOCKET, SO_REUSEPORT,
                 &optval, static_cast<socklen_t>(sizeof(optval)));

    if(ret < 0 && on) {
        LOG_SYSERR << "SO_REUSEPORT failed!";
    }
}

/*
void Socket::setTcpNoDelay(bool on) {
    int val = on ? 1 : 0;
    ::setsockopt(_sockFd, IPPROTO_TCP, TCP_NODELAY, &val, static_cast<socklen_t>(sizeof val));
}
*/