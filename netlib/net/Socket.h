#ifndef SOCKET_H_
#define SOCKET_H_

/*
 *  封装套接字
 *  bind()/listen()/accept()/
 */

namespace netlib
{

class SockAddr;

class Socket
{
public:
    explicit Socket(int fd) : _sockFd(fd) {}
    ~Socket();

    int getSocketFd() const {
        return _sockFd;
    }

    void bindSockAddr(const SockAddr &localAddr);
    void listen();
    
    int accept(SockAddr *peerAddr);

    void shutdownWrite();

    void setKeepAlive(bool on);

    void setReuseAddr(bool on);
    
    void setReusePort(bool on);

    //void setTcpNoDelay(bool on);

private:
    int _sockFd;
};

}

#endif