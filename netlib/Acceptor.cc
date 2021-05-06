#include "./Acceptor.h"
#include "./SockFunc.h"
#include "./SockAddr.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace netlib;


Acceptor::Acceptor(EventLoop *loop, SockAddr &listenAddr) 
    : _ownLoop(loop),
      _acceptSocket(netlib::createSocketFd(listenAddr.getFamily())),
      _acceptChnnel(loop, _acceptSocket.getSocketFd()),
      _listening(false),
      _idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    _acceptSocket.bindSockAddr(listenAddr);
    _acceptChnnel.setReadCallBack(std::bind(&Acceptor::handleRead, this));
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
        if(errno == EMFILE) {
            /// 已经到达了每个进程所能打开的文件描述符的限制
            ::close(_idleFd);
            _idleFd = ::accept(_acceptSocket.getSocketFd(), nullptr, nullptr);
            ::close(_idleFd);
            _idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}