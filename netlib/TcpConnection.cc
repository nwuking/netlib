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
      _chnnel(new Chnnel(loop, sockfd)),
      _inputBuffer(),
      _outputBuffer()
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

    ssize_t n = _inputBuffer.readFd(_chnnel->getFd());

    if(n > 0) {
        /// 接收到消息，调用回调函数
        /// TcpConnection 由 TcpServer管理
        /// 在TcpServer中，TcpConnection实例化对象由智能指针管理
        /// 使用shared_from_this()能够对象自身以智能指针形式传给自身的函数
        /// 且不会使智能指针的引用计数增加
        /// 使用shared_from_this(),需要继承enabled_shared_from_this类
        /// 或者自己造一个轮子
        _messageCallBack(shared_from_this(), &_inputBuffer);
    }
    else if(n == 0) {
        handleClose();
    }
    else {
        /// error
        handleError();
    }
}

void TcpConnection::handleWrite() {
    /// 有数据将要发送给client

    _loop->assertInLoopThread();
}