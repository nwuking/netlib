#include "./TcpConnection.h"
#include "./EventLoop.h"
#include "./Chnnel.h"


using namespace netlib;
using namespace std::placeholders;

TcpConnection::TcpConnection(EventLoop *loop,
                             int sockfd,
                             const SockAddr &peerAddr,
                             const SockAddr &localAddr)
    : _loop(loop),
      _peerAddr(peerAddr),
      _localAddr(localAddr),
      _chnnel(new Chnnel(loop, sockfd))
{
    /// 注册_chnnel
    _chnnel->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
    _chnnel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
    _chnnel->setErrorCallBack(std::bind(&TcpConnection::handleError, this));
    _chnnel->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
}


void TcpConnection::handleRead() {
    /// 有client数据到来的时候
    /// 调用此函数读取
    _loop->assertInLoopThread();

    // read(_chnnel->fd())
}