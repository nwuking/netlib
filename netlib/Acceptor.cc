#include "./Acceptor.h"

using namespace netlib;


Acceptor::Acceptor(EventLoop *loop, SockAddr &listenAddr) 
    : _ownLoop(loop)
{
}