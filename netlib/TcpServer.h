#ifndef TCPSERVER_H_
#define TCPSERVER_H_

/// TcpServer-> 用于服务器端的开发
///             服务器端的开发大概为：socket()-> bind()-> listen()-> accept()

#include "./SockAddr.h"

namespace netlib
{

class EventLoop;

class TcpServer
{
public:
    TcpServer(EventLoop *loop, SockAddr &listenAddr);
    ~TcpServer();

private:
    EventLoop *_loop;
    SockAddr _listenAddr;
};

}

#endif