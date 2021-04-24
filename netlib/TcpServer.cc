#include "./TcpServer.h"
#include "./EventLoop.h"
#include "./Acceptor.h"
#include "./EventLoopThreadPool.h"
#include "./SockFunc.h"


using namespace netlib;
using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *loop, SockAddr &listenAddr)
    : _loop(loop),
      _listenAddr(listenAddr),
      _acceptor(new Acceptor(loop, listenAddr))
{
  _acceptor->setNewConnectionCallBack(std::bind(&TcpServer::newConnection, this, _1, _2));
}


void TcpServer::newConnection(int fd, SockAddr &peerAddr) {
  /// fd->  accept()产生的描述符
  /// peerAddr->  客户端的地址结构
  /// 根据(fd, peerAddr)创建一个TcpConnection对象
  /// 从线程池中get一个线程，将TcpConnection对象丢到该线程中

  _loop->assertInLoopThread();
  EventLoop *threadLoop = _threadPool->getNextLoop();

  /// 生成一个TcpConnection对象
  SockAddr localAddr(netlib::getLocalAddr(fd));
  //TcpConnectionPtr conn = new TcpConnection();
}