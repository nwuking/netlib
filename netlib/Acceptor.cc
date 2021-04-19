#include "./Acceptor.h"
#include "./SockFunc.h"
#include "./SockAddr.h"

using namespace netlib;


Acceptor::Acceptor(EventLoop *loop, SockAddr &listenAddr) 
    : _ownLoop(loop),
      _acceptSocket(netlib::createSocketFd(listenAddr.getFamily())),
      _acceptChnnel(loop, _acceptSocket.getSocketFd()),
      _listening(false)
{
    _acceptSocket.bindSockAddr(listenAddr);
    _acceptChnnel.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::handleRead() {
    /// 该函数与监听套接字挂钩
    /// 当有client到来，该函数被调用
    SockAddr peerAddr;
    int n = _acceptSocket.getSocketFd();
}