#include "./TcpClient.h"
#include "./Logging.h"
#include "./Connector.h"
#include "./EventLoop.h"
#include "./SockFunc.h"

#include <stdio.h>

namespace netlib
{

namespace yyj
{

void removeConnection(EventLoop *loop, const TcpConnectionPtr &conn) {
    loop->queueInLoop(std::bind(&TcpConnection::conncetDestoryed, conn));
}

}

}

using namespace netlib;

TcpClient::TcpClient(EventLoop *loop,
                     const SockAddr &addr,
                     const std::string &name)
    : _loop(loop),
      _name(name),
      _connector(new Connector(loop, addr)),
      _connect(true),
      _retry(false),
      _nextConnId(0)
{
    _connector->setNewConnectionCallBack(std::bind(
                &TcpClient::NewConnection, this, std::placeholders::_1));

    LOG_INFO << "TcpClient::TcpClient()[" << _name << "] -connector" << _connector.get();
}

TcpClient::~TcpClient() {
    LOG_INFO << "TcpClient::~TcpClient()[" << _name << "] connector " << _connector.get();

    TcpConnectionPtr conn;
    bool unique;
    {
        MutexLock lock(_mutex);
        unique = _connection.unique();
        conn = _connection;
    }

    if(conn) {
        assert(_loop == conn->getLoop());
        CloseCallBack cb = std::bind(&yyj::removeConnection, _loop, std::placeholders::_1);
        _loop->runInLoop(std::bind(&TcpConnection::setCloseCallBack, conn, cb));

        if(unique) {
            conn->forceClose();
        }
    }
    else {
        _connector->stop();
    }
}

void TcpClient::connect() {
    /// 发起连接
    LOG_INFO << "TcpClient::connect [" << _name << "] -connecting to "
             << _connector->serverAddress().toIpPort();

    _connect = true;
    _connector->start();
}

void TcpClient::NewConnection(int fd) {
    _loop->assertInLoopThread();
    SockAddr peerAddr(netlib::getpeerAddr(fd));
    char buf[32];
    ::snprintf(buf, sizeof buf, "%s#%d", peerAddr.toIpPort().c_str(), _nextConnId);
    ++_nextConnId;
    std::string connName = _name + buf;

    SockAddr localAddr(netlib::getLocalAddr(fd));

    TcpConnectionPtr conn(new TcpConnection(_loop, fd, peerAddr, 
                                            localAddr, connName));

    conn->setConnectionCallBack(_connectionCallBack);
    conn->setWriteCompleteCallBack(_writeCompleteCallBack);
    conn->setMessageCallBack(_messageCallBack);
    conn->setCloseCallBack(std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));
    {
        MutexLock lock(_mutex);
        _connection = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr &conn) {
    _loop->assertInLoopThread();
    assert(_loop == conn->getLoop());

    {
        MutexLock lock(_mutex);
        assert(_connection == conn);
        _connection.reset();
    }

    _loop->queueInLoop(std::bind(&TcpConnection::conncetDestoryed, conn));

    if(_retry && _connect) {
        LOG_INFO << "TcpClient::connect[" << _name << "] - Reconnecting to " 
                 << _connector->serverAddress().toIpPort();

        _connector->restart();
    }
}