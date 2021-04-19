#ifndef TCPSERVER_H_
#define TCPSERVER_H_

/// TcpServer-> 用于服务器端的开发
///             服务器端的开发大概为：socket()-> bind()-> listen()-> accept()

#include "./SockAddr.h"

#include <memory>

namespace netlib
{

class EventLoop;
class Acceptor;

class TcpServer
{
public:
    TcpServer(EventLoop *loop, SockAddr &listenAddr);
    ~TcpServer();

private:
    void newConnect(int fd, SockAddr &peerAddr);

    EventLoop *_loop;
    SockAddr _listenAddr;

    std::unique_ptr<Acceptor> _acceptor;                    /// 用于监听，等待client的到来，只有一个
};

}

#endif