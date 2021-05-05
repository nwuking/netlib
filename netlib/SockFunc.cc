#include "./SockFunc.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace netlib;


void setNoBlockAndCloseOnExec(int fd) {
    /// 将描述符设置为非阻塞和close on exec
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    flags |= O_CLOEXEC;
    int ret = ::fcntl(fd, F_SETFL, flags);
    (void)ret;
}


void hostToNetwork32(struct in_addr *addr) {
    struct in_addr res;
    int n = ::inet_aton((const char*)addr, &res);
    if(n) {
        ::memcpy(addr, &res, sizeof res);
    }
    else {
        /// n == 0, addr 无效
    }
}

void hostToNetwork16(uint16_t *port, uint16_t por) {
    *port = ::htons(por);
    if(!port) {
        ///  failed
    }
}

void fromIp(struct in_addr *addr, const char *ip) {
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