#include "./TcpConnection.h"
#include "./EventLoop.h"
#include "./Chnnel.h"
#include "./SockFunc.h"
#include "./Socket.h"


using namespace netlib;
using namespace std::placeholders;

TcpConnection::TcpConnection(EventLoop *loop,
                             int sockfd,
                             const SockAddr &peerAddr,
                             const SockAddr &localAddr,
                             const std::string &name)
    : _loop(loop),
      _peerAddr(peerAddr),
      _localAddr(localAddr),
      _name(name),
      _socket(new Socket(sockfd)),
      _chnnel(new Chnnel(loop, sockfd)),
      _inputBuffer(),
      _outputBuffer(),
      _state(cConnecting)
{
    /// 注册_chnnel
    _chnnel->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
    _chnnel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
    _chnnel->setErrorCallBack(std::bind(&TcpConnection::handleError, this));
    _chnnel->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
}

void TcpConnection::conncetDestoryed() {
    _loop->assertInLoopThread();

    if(_state == cConnected) {
        setState(cDisconnected);
        _chnnel->diableAll();
    }

    _chnnel->remove();
}

void TcpConnection::connectEstablished() {
    _loop->assertInLoopThread();
    assert(_state == cConnecting);
    setState(cConnected);

    _chnnel->tie(shared_from_this());
    _chnnel->enableReading();
}

void TcpConnection::shutdownInLoop() {
    /// 服务器端关闭连接
    /// 此时，服务端没有数据发送且关闭后不能发送数据
    /// 可以接收数据

    _loop->assertInLoopThread();
    if(!_chnnel->isWriting()) {
        /// 没有数据要写
        _socket->shutdownWrite();
    }
}

void TcpConnection::handleRead() {
    /// 有client数据到来的时候
    /// 调用此函数读取
    _loop->assertInLoopThread();

    ssize_t n = _inputBuffer.readFd(_chnnel->getFd());

    if(n > 0) {
        /// 接收到消息，调用业务逻辑函数
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
    if(_chnnel->isWriting()) {
        /// 写数据，有数据要发送给client端
        ssize_t n = netlib::write(_chnnel->getFd(), _outputBuffer.peek(), _outputBuffer.readAbleBytes());

        if(n > 0) {
            _outputBuffer.retrieve(n);

            if(_outputBuffer.readAbleBytes() == 0) {
                /// 数据已经发送完了，_outputBuffer为空
                /// 注销可写事件
                _chnnel->disableWriting();

                if(_writeCompleteCallBack) {
                    /// 将_writeCompleteCallBack放进_loop的回调函数队列(_pendingFunctors)
                    /// 当_loop被唤醒的时候才执行_pendingFunctors中的回调函数

                    /// std::function对象可以使用bind成为另一个std::function对象
                    _loop->queueInLoop(std::bind(_writeCompleteCallBack, shared_from_this()));
                }

                if(_state == cDisconnecting) {
                    /// server要断开连接
                    /// Tcp的断开是单方面的
                    shutdownInLoop();
                }
            }
        }
        else {
            /// 写数据失败
            ;
        }
    }
    else {
        /// 没有数据写
        ;
    }
}

void TcpConnection::handleError() {
    /// epoll监听失败的时候调用
    int err = netlib::getSocketError(_chnnel->getFd());

    ///////
}

void TcpConnection::handleClose() {
    /// 
}