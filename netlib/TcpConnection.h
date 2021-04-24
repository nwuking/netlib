#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

/*
 *  封装accept()返回的描述符和客户端的地址结构
 *  表示server和client的一条链接
 */

#include "./SockAddr.h"

#include <memory>

namespace netlib
{

class EventLoop;

class TcpConnection
{
public:
    TcpConnection(EventLoop *loop, int sockfd, SockAddr &peerAddr);
    ~TcpConnection();
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}

#endif