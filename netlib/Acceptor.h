#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

/*
 *  管理一个监听套接字，等待客户端的到来
 */

#include "./EventLoop.h"
#include "./SockAddr.h"

namespace netlib
{

class Acceptor
{
public:
    Acceptor(EventLoop *loop, SockAddr &listenAddr);
    ~Acceptor();

private:
    EventLoop *_ownLoop;
    //Socket _acceptSocket;
};

}

#endif