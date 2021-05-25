#include "./Connector.h"
#include "./Logging.h"
#include "./EventLoop.h"
#include "./Chnnel.h"
#include "./SockFunc.h"

#include <assert.h>

using namespace netlib;

Connector::Connector(EventLoop *loop, const SockAddr &addr)
    : _loop(loop),
      _addr(addr),
      _connect(false),
      _state(cDisconnected),
      _retryDelayMs(cInitRetryDelayMs)
{
    LOG_DEBUG << "Connector::Connector() [" << this << "]";
}

Connector::~Connector() {
    LOG_DEBUG << "Connector::~Connector() [" << this << "]";
    assert(!_chnnel);
}

void Connector::start() {
    _connect = true;
    _loop->runInLoop(std::bind(&Connector::startInLoop, this));
}

void Connector::restart() {
    _loop->assertInLoopThread();
    setState(cDisconnected);
    _retryDelayMs = cInitRetryDelayMs;
    _connect = true;
    startInLoop();
}

void Connector::stop() {
    _connect = false;
    _loop->queueInLoop(std::bind(&Connector::stopInLoop, this));
}

void Connector::stopInLoop() {
    _loop->assertInLoopThread();
    if(_state == cConnecting) {
        setState(cDisconnected);
        int fd = removeAndResetChnnel();
        retry(fd);
    }
}

void Connector::startInLoop() {
    _loop->assertInLoopThread();
    assert(_state == cDisconnected);
    if(_connect) {
        /// 发起连接请求
        connect();
    }
    else {
        LOG_DEBUG << "do not connect";
    }
}

void Connector::connect() {
    int sockFd = netlib::createNoblockingOrDie(_addr.getFamily());
    int ret = netlib::connect(sockFd, _addr.getSockAddr());

    int savedError = (ret == 0) ? 0 : errno;
    switch(savedError) {
        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN:
            connecting(sockFd);
            break;

        case EAGAIN:
        case EADDRINUSE:
        case EADDRNOTAVAIL:
        case ECONNREFUSED:
        case ENETUNREACH:
            retry(sockFd);
            break;
        
        case EACCES:
        case EPERM:
        case EAFNOSUPPORT:
        case EALREADY:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
            LOG_SYSERR << "error in Connector::statInLoop() " << savedError;
            netlib::close(sockFd);
            break;
        default:
            LOG_SYSERR << "Unexpect error in Connector::startInLoop " << savedError;
            netlib::close(sockFd);
            break;
    }
}

void Connector::connecting(int sockFd) {
    /// TCP连接已经连上，将事件(Chnnel)注册到EventLoop

    setState(cConnecting);
    assert(!_chnnel);
    _chnnel.reset(new Chnnel(_loop, sockFd));
    _chnnel->setWriteCallBack(std::bind(&Connector::handleWrite, this));
    _chnnel->setErrorCallBack(std::bind(&Connector::handleError, this));
    _chnnel->enableWriting();
}

int Connector::removeAndResetChnnel() {
    _chnnel->diableAll();
    _chnnel->remove();
    int fd = _chnnel->getFd();
    _loop->queueInLoop(std::bind(&Connector::resetChnnel, this));
    return fd;
}

void Connector::resetChnnel() {
    _chnnel.reset();
}

void Connector::handleWrite() {
    /// 因为是非阻塞的connect()
    /// 所以只有当sockFd可读的时候，才真正表示连接成功
    /// 即只有该函数被调用的时候

    LOG_TRACE << "Connector::handleWrite() " << _state;

    if(_state == cConnecting) {
        int sockFd = removeAndResetChnnel();
        int err = netlib::getSocketError(sockFd);

        if(err) {
            LOG_WARN << "Connector::handleWrite() - SO_ERROR = " << err
                     << " " << ::strerror(err);
            
            retry(sockFd);
        }
        else if(netlib::isSelfConnect(sockFd)) {
            LOG_WARN << "Connector::handleWrite() - self connect";
            retry(sockFd);
        }
        else {
            /// 连接成功
            setState(cConnected);
            if(_connect) {
                _newConnectionCallBack(sockFd);
            }
            else {
                netlib::close(sockFd);
            }
        }
    }
    else {
        setState(cDisconnected);
    }
}

void Connector::handleError() {
    LOG_ERROR << "Connector::handleError() " << _state;

    if(_state == cConnecting) {
        int sockFd = removeAndResetChnnel();
        int err = netlib::getSocketError(sockFd);
        LOG_TRACE << "SO_ERROR " << err << ::strerror(err);
        retry(sockFd);
    }
}

void Connector::retry(int fd) {
    /// 重新发起连接
    netlib::close(fd);
    setState(cDisconnected);
    if(_connect) {
        LOG_INFO << "Connector::retry - Retry to connecte " << _addr.toIpPort()
                 << " in " << _retryDelayMs << "microSeconds";

        _loop->runAfter(_retryDelayMs/1000.0,
                        std::bind(&Connector::startInLoop, shared_from_this()));

        _retryDelayMs = std::min(_retryDelayMs * 2, cMaxRetryDelayMs);
    }
    else {
        LOG_DEBUG << "do not connect!";
    }
}
