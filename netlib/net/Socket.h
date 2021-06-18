#ifndef SOCKET_H_
#define SOCKET_H_

#include "netlib/base/noncopyable.h"
#include <stdlib.h>

/*
 *  封装套接字
 *  bind()/listen()/accept()/
 */

struct tcp_info;

namespace netlib
{

class SockAddr;

class Socket :NonCopyAble
{
public:
    explicit Socket(int fd) : _sockFd(fd) {}
    ~Socket();

    int getSocketFd() const {
        return _sockFd;
    }

    bool getTcpInfo(struct tcp_info* tcpi) const;
    bool getTcpInfoString(char *buf, size_t len) const;

    void bindSockAddr(const SockAddr &localAddr);
    void listen();
    
    int accept(SockAddr *peerAddr);

    void shutdownWrite();

    void setKeepAlive(bool on);

    void setReuseAddr(bool on);
    
    void setReusePort(bool on);

    void setTcpNoDelay(bool on);

private:
    const int _sockFd;
};

}

#endif