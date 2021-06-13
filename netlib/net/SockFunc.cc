#include "netlib/net/SockFunc.h"
#include "netlib/base/Logging.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>


using namespace netlib;


void setNoBlockAndCloseOnExec(int fd) {
    /// 将描述符设置为非阻塞和close on exec
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    flags |= O_CLOEXEC;
    int ret = ::fcntl(fd, F_SETFL, flags);
    (void)ret;
}

uint16_t netlib::networkToHost16(uint16_t port) {
    return ::ntohs(port);
}

void netlib::hostToNetwork32(struct in_addr *addr) {
    in_addr_t t = addr->s_addr;
    addr->s_addr = static_cast<in_addr_t>(::htonl(t));
}

void netlib::hostToNetwork16(uint16_t *port, uint16_t por) {
    *port = ::htons(por);
}

void netlib::fromIp(struct in_addr *addr, const char *ip) {
    int n = ::inet_aton(ip, addr);
    if(n == 0) {
        /// failed
    }
}


void netlib::close(int fd) {
    int n = ::close(fd);
    if(n < 0) {
        /// n == 0, 表示成功返回
    }
}
void netlib::bind(int fd, const struct sockaddr *addr) {
    int n = ::bind(fd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
    if(n < 0) {
        /// error
    }
}

void netlib::listen(int fd) {
    int n = ::listen(fd, SOMAXCONN);
    if(n < 0) {
        /// error
    }
}

int netlib::accept(int fd, struct sockaddr_in *addr) {
    socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
    int newFd = ::accept(fd, (sockaddr*)addr, &len);

    /// 得到一个与客户端关联的描述符newFd和客户端的地址结构addr
    setNoBlockAndCloseOnExec(newFd);                       /// 将newFd设置为NONBLOCK
    if(newFd < 0) {
        /// error
    }
    return newFd;
}

int createSocketFd(sa_family_t family) {
    int n = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(n < 0) {
        /// error
    }
    return n;
}


struct sockaddr_in getLocalAddr(int sockfd) {
    /// sockfd:accept()返回的描述符
    /// 获取与客户建立连接的本地地址
    struct sockaddr_in localAddr;
    ::bzero(&localAddr, sizeof localAddr);
    socklen_t len = static_cast<socklen_t>(sizeof localAddr);

    if(::getsockname(sockfd, (struct sockaddr*)&localAddr, &len) < 0) {
        /// 调用失败
    }
    return localAddr;
}

ssize_t netlib::readv(int fd, const struct iovec *iov, int iovcnt) {
    /// 调用readv(), scatter read
    return ::readv(fd, iov, iovcnt);
}

ssize_t netlib::write(int fd, const void *buf, size_t len) {
    return ::write(fd, buf, len);
}

ssize_t netlib::read(int fd, void *buf, size_t len) {
    return ::read(fd, buf, len);
}

void netlib::shutdownWrite(int fd) {
    /// call  shutdown(...)
    if(::shutdown(fd, SHUT_WR) < 0) {
        /// error
        ;
    }
}

int netlib::getSocketError(int fd) {
    /// 获取描述符出现的错误
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if(::getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
        /// 调用出错
        return errno;
    }
    else {
        /// 
        return optval;
    }
}

void netlib::toIpPort(char *buf, size_t size, const struct sockaddr_in *addr) {
    toIp(buf, size, addr);
    size_t end = ::strlen(buf);
    uint16_t port = netlib::networkToHost16(addr->sin_port);
    assert(size > end);
    ::snprintf(buf+end, size-end, ":%u", port);
}

void netlib::toIp(char *buf, size_t size, const struct sockaddr_in *addr) {
    if(addr->sin_family == AF_INET) {
        assert(size >= INET_ADDRSTRLEN);
        ::inet_ntop(AF_INET, &addr->sin_addr, buf, static_cast<socklen_t>(size));
    }
}

int netlib::createNoblockingOrDie(sa_family_t family) {
    int sockFd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC , IPPROTO_TCP);
    if(sockFd < 0) {
        LOG_SYSFATAL << "SockFunc::createNoblockingOrDie";
    }

    return sockFd;
}

int netlib::connect(int sockFd, const struct sockaddr_in *addr) {
    return ::connect(sockFd,(struct sockaddr*)(addr),
                     static_cast<socklen_t>(sizeof(struct sockaddr_in)));
}

struct sockaddr_in netlib::getLocalAddr(int sockFd) {
    struct sockaddr_in localAddr;
    ::bzero(&localAddr, sizeof localAddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localAddr);
    if(::getsockname(sockFd, (struct sockaddr*)(&localAddr), &addrlen) < 0) {
        LOG_SYSERR << "SockFunc::getLocalAddr()";
    }
    return localAddr;
}


struct sockaddr_in netlib::getpeerAddr(int sockFd) {
    struct sockaddr_in peerAddr;
    ::bzero(&peerAddr, sizeof peerAddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof peerAddr);
    if(::getpeername(sockFd, (struct sockaddr*)(&peerAddr), &addrlen) < 0) {
        LOG_SYSERR << "SockFunc::getLocalAddr()";
    }
    return peerAddr;
}

bool netlib::isSelfConnect(int sockFd) {
    struct sockaddr_in localAddr = getLocalAddr(sockFd);
    struct sockaddr_in peerAddr = getpeerAddr(sockFd);

    if(localAddr.sin_family == AF_INET) {
        return localAddr.sin_addr.s_addr == peerAddr.sin_addr.s_addr
                && localAddr.sin_port == peerAddr.sin_port;
    }
    else {
        return false;
    }
}