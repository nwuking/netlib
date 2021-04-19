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
  _acceptor->setNewConnectionCallBack(std::bind(&TcpServer::newConnect, this, _1, _2));
}


void TcpServer::newConnect(int fd, SockAddr &peerAddr) {
  /// 
  
}