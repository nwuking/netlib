#include "netlib/net/TcpConnection.h"
#include "netlib/net/EventLoop.h"
#include "netlib/net/Chnnel.h"
#include "netlib/net/SockFunc.h"
#include "netlib/net/Socket.h"
#include "netlib/base/Logging.h"


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

    LOG_DEBUG << "TcpConnection::TcpConnection() -[" << _name << "] at "
              << this << " fd = " << _socket->getSocketFd();

    _socket->setKeepAlive(true);
}

TcpConnection::~TcpConnection() {
    LOG_DEBUG << "TcpConnection::~TcpConnection() -[ " << _name << " ] at "
              << this  << " fd = " << _chnnel->getFd() 
              << " state = " << stateToString();
    
    assert(_state == cDisconnected);
}

void TcpConnection::shutdown() {
    if(_state == cConnected) {
        setState(cDisconnecting);
        _loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

bool TcpConnection::getTcpInfo(struct tcp_info *tcpi) const {
    return _socket->getTcpInfo(tcpi);
}

std::string TcpConnection::getTcpInfoString() const {
    char buf[1024];
    buf[0] = '\0';
    _socket->getTcpInfoString(buf, sizeof buf);
    return buf;
}

void TcpConnection::send(const void *message, int len) {
    send(std::string(static_cast<const char*>(message), len));
}

void TcpConnection::send(const std::string &message) {
    if(_state == cConnected) {
        if(_loop->isInLoopThread()) {
            sendInLoop(message);
        }
        else {
            ///////
        }
    }
}

void TcpConnection::forceClose() {
    if(_state == cConnected || _state == cDisconnecting) {
        setState(cDisconnecting);
        _loop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
    }
}

void TcpConnection::forceCloseInLoop() {
    _loop->assertInLoopThread();
    if(_state == cConnected || _state == cDisconnecting) {
        handleClose();
    }
}

void TcpConnection::conncetDestoryed() {
    _loop->assertInLoopThread();

    if(_state == cConnected) {
        setState(cDisconnected);
        _chnnel->diableAll();

        _connectionCallBack(shared_from_this());
    }

    _chnnel->remove();
}

void TcpConnection::connectEstablished() {
    _loop->assertInLoopThread();
    assert(_state == cConnecting);
    setState(cConnected);

    _chnnel->tie(shared_from_this());
    _chnnel->enableReading();

    _connectionCallBack(shared_from_this());
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
    int savedErrno = 0;
    ssize_t n = _inputBuffer.readFd(_chnnel->getFd(), &savedErrno);

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
        errno = savedErrno;
        LOG_SYSERR << "TcpConnection::handleRead()";
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
            LOG_SYSERR << "TcpConnection::handleWrite()";
        }
    }
    else {
        /// 没有数据写
        LOG_TRACE << "TcpConnection fd = " << _chnnel->getFd()
                  << " is down, no more writing";
    }
}

void TcpConnection::handleError() {
    /// epoll监听失败的时候调用
    int err = netlib::getSocketError(_chnnel->getFd());

    LOG_ERROR << "TcpConnection::handleError [" << _name 
              << " ] -SO_ERROR " << err << " " << ::strerror(err);
}

void TcpConnection::handleClose() {
    _loop->assertInLoopThread();

    LOG_TRACE << "fd = " << _chnnel->getFd() << " state : " << stateToString();

    assert(_state == cDisconnecting || _state == cDisconnected);
    setState(cDisconnected);
    _chnnel->diableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    _connectionCallBack(guardThis);
    _closeCallBack(guardThis);
}

const char* TcpConnection::stateToString() const {
    switch(_state) {
        case cDisconnected :
            return "Disconnected";
        case cDisconnecting :
            return "Disconnecting";
        case cConnected :
            return "Connected";
        case cConnecting :
            return "Connecting";
        default :
            return "unknown state";
    }
}