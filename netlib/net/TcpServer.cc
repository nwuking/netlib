#include "netlib/net/TcpServer.h"
#include "netlib/net/EventLoop.h"
#include "netlib/net/Acceptor.h"
#include "netlib/net/EventLoopThreadPool.h"
#include "netlib/net/SockFunc.h"
#include "netlib/base/Logging.h"


using namespace netlib;
using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *loop, SockAddr &listenAddr, 
                    const std::string &name, Option option)
    : _loop(loop),
      _ipPort(listenAddr.toIpPort()),
      _listenAddr(listenAddr),
      _acceptor(new Acceptor(loop, listenAddr, option == cReusePort)),
      _threadPool(new EventLoopThreadPool(loop, name)),
      _name(name),
      _nextConnId(1),
      _started(false)
{
  _acceptor->setNewConnectionCallBack(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer() {
  _loop->assertInLoopThread();

  LOG_TRACE << "TcpServer::~TcpServer [" << _name << "] ";

  for(auto &item : _connections) {
    TcpConnectionPtr conn(item.second);     /// 引用计数+1
    item.second.reset();                    /// 引用计数-1

    conn->getLoop()->runInLoop(
        std::bind(&TcpConnection::conncetDestoryed, conn));
  }
}

void TcpServer::start() {
  if(!_started) {
    /// 运行线程池
    _started = true;
    _threadPool->start(_threadInitCallBack);

    assert(!_acceptor->listening());
    _loop->runInLoop(std::bind(&Acceptor::listen, _acceptor.get()));
  }
}

void TcpServer::setThreadNums(int num) {
  assert(num >= 0);
  _threadPool->setNumThreads(num);
}


void TcpServer::newConnection(int fd, const SockAddr &peerAddr) {
  /// fd->  accept()产生的描述符
  /// peerAddr->  客户端的地址结构
  /// 根据(fd, peerAddr)创建一个TcpConnection对象
  /// 从线程池中get一个线程，将TcpConnection对象丢到该线程中

  _loop->assertInLoopThread();
  EventLoop *threadLoop = _threadPool->getNextLoop();

  std::string conName = _name + std::to_string(_nextConnId);
  ++_nextConnId;

  LOG_INFO << "TcpServer::newConnection [" << _name
           << "] - new connection [" << conName
           << "] from " << peerAddr.toIpPort();

  /// 生成一个TcpConnection对象
  SockAddr localAddr(netlib::getLocalAddr(fd));
  TcpConnectionPtr conn(new TcpConnection(threadLoop, fd, peerAddr, localAddr, conName));
  _connections[conName] = conn;

  conn->setMessageCallBack(_messageCallBack);
  conn->setWriteCompleteCallBack(_writeCompleteCallBack);
  conn->setConnectionCallBack(_connectionCallBack);
  conn->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, _1));

  threadLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
  _loop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn) {
  _loop->assertInLoopThread();

  LOG_INFO << "TcpServer::removeConnectionInLoop [" << _name
           << "] - connection " << conn->name();

  size_t n = _connections.erase(conn->name());
  (void)n;
  assert(n == 1);

  EventLoop *ioLoop = conn->getLoop();
  ioLoop->queueInLoop(std::bind(&TcpConnection::conncetDestoryed, conn));
}