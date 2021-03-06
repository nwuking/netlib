#include "netlib/net/Acceptor.h"
#include "netlib/net/SockFunc.h"
#include "netlib/net/SockAddr.h"
#include "netlib/base/Logging.h"
#include "netlib/net/EventLoop.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

using namespace netlib;


Acceptor::Acceptor(EventLoop *loop, const SockAddr &listenAddr, bool reuseport) 
    : _ownLoop(loop),
      _acceptSocket(netlib::createNoblockingOrDie(listenAddr.getFamily())),
      _acceptChnnel(loop, _acceptSocket.getSocketFd()),
      _listening(false),
      _idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    assert(_idleFd > 0);
    /// 在bind之前设置
    _acceptSocket.setReuseAddr(true);
    _acceptSocket.setReusePort(reuseport);
    _acceptSocket.bindSockAddr(listenAddr);
    _acceptChnnel.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    _acceptChnnel.diableAll();
    _acceptChnnel.remove();
    ::close(_idleFd);
}

void Acceptor::listen() {
    _ownLoop->assertInLoopThread();

    _listening = true;
    _acceptSocket.listen();
    _acceptChnnel.enableReading();
}

void Acceptor::handleRead() {
    /// 该函数与监听套接字挂钩
    /// 当有client到来，该函数被调用
    _ownLoop->assertInLoopThread();
    SockAddr peerAddr;
    int connFd = _acceptSocket.accept(&peerAddr);
    if(connFd >= 0) {
        //
        if(_newConnectionCallBack) {
            /// 该函数对象由TcpServer设置
            /// 将为每一个到来的连接创建一个Connector对象
            _newConnectionCallBack(connFd, peerAddr);
        }
        else {
            netlib::close(connFd);
        }
    }
    else {
        /// error
        LOG_SYSERR << "in Acceptor::handleRead()";
        if(errno == EMFILE) {
            /// 已经到达了每个进程所能打开的文件描述符的限制
            ::close(_idleFd);
            _idleFd = ::accept(_acceptSocket.getSocketFd(), NULL, NULL);
            ::close(_idleFd);
            _idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}