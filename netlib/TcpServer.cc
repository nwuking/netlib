#include "./TcpServer.h"
#include "./EventLoop.h"
#include "./Acceptor.h"


using namespace netlib;

TcpServer::TcpServer(EventLoop *loop, SockAddr &listenAddr)
    : _loop(loop),
      _listenAddr(listenAddr),
      _acceptor(new Acceptor(loop, listenAddr))
{
}