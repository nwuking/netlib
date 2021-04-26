#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

/*
 *  封装accept()返回的描述符和客户端的地址结构
 *  表示server和client的一条链接
 */

#include "./SockAddr.h"
#include "./Time.h"
#include "./Buffer.h"

#include <memory>

namespace netlib
{

class EventLoop;
class Chnnel;

class TcpConnection
{
public:
    TcpConnection(EventLoop *loop, int sockfd, const SockAddr &peerAddr,
                  const SockAddr &localAddr);
    ~TcpConnection();

private:
    //void handleRead(Time recviveTime);
    void handleRead();
    void handleWrite();
    void handleError();
    void handleClose();

    EventLoop *_loop;
    const SockAddr _peerAddr;
    const SockAddr _localAddr;
    std::unique_ptr<Chnnel> _chnnel;

    Buffer _inputBuffer;                        /// 用于数据的读
    Buffer _outputBuffer;                       /// 用于数据的写
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}

#endif