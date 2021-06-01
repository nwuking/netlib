#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

/*
 *  管理一个监听套接字，等待客户端的到来
 */

#include "netlib/net/EventLoop.h"
#include "netlib/net/Chnnel.h"
#include "netlib/net/SockAddr.h"
#include "netlib/net/Socket.h"

namespace netlib
{

class Acceptor : NonCopyAble
{
public:
    typedef std::function<void(int fd, SockAddr&)> NewConnectionCallBack;

    Acceptor(EventLoop *loop, SockAddr &listenAddr, bool reuseport);
    ~Acceptor();

    void listen();

    void setNewConnectionCallBack(NewConnectionCallBack cb) {
        _newConnectionCallBack = cb;
    }

    bool listening() const {
        return _listening;
    }

private:
    void handleRead();

    EventLoop *_ownLoop;
    Socket _acceptSocket;
    Chnnel _acceptChnnel;
    bool _listening;
    int _idleFd;

    NewConnectionCallBack _newConnectionCallBack;
};

}

#endif