#include "./SockFunc.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

using namespace netlib;


static void setNoBlockAndCloseOnExec(int fd) {
    /// 将描述符设置为非阻塞和close on exec
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

    setNoBlockAndCloseOnExec(newFd);
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