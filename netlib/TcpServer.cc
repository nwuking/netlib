#include "./TcpServer.h"
#include "./EventLoop.h"
#include "./Acceptor.h"
#include "./EventLoopThreadPool.h"
#include "./SockFunc.h"


using namespace netlib;
using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *loop, SockAddr &listenAddr, const std::string &name)
    : _loop(loop),
      _listenAddr(listenAddr),
      _acceptor(new Acceptor(loop, listenAddr)),
      _name(name),
      _nextConnId(0),
      _started(false)
{
  _acceptor->setNewConnectionCallBack(std::bind(&TcpServer::newConnection, this, _1, _2));
}

void TcpServer::start() {
  if(!_started) {
    /// 运行线程池
    _threadPool->start(_threadInitCallBack);

    assert(!_acceptor->listening());
    _loop->runInLoop(std::bind(&Acceptor::listen, _acceptor.get()));
    _started = true;
  }
}

void TcpServer::setThreadNums(int num) {
  assert(num >= 0);
  _threadPool->setNumThreads(num);
}


void TcpServer::newConnection(int fd, SockAddr &peerAddr) {
  /// fd->  accept()产生的描述符
  /// peerAddr->  客户端的地址结构
  /// 根据(fd, peerAddr)创建一个TcpConnection对象
  /// 从线程池中get一个线程，将TcpConnection对象丢到该线程中

  _loop->assertInLoopThread();
  EventLoop *threadLoop = _threadPool->getNextLoop();

  std::string conName = _name + std::to_string(_nextConnId);
  ++_nextConnId;

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

  size_t n = _connections.erase(conn->name());
  (void)n;
  assert(n == 1);

  EventLoop *ioLoop = conn->getLoop();
  ioLoop->queueInLoop(std::bind(&TcpConnection::conncetDestoryed, conn));
}