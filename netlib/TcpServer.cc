#include "./TcpServer.h"
#include "./EventLoop.h"
#include "./Acceptor.h"


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
  /// 根据(fd, peerAddr)创建一个Connector对象
  /// 从线程池中get一个线程，将Connector对象丢到该线程中

  //EventLoop *threadLoop = 
}