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
class Socket;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void(const TcpConnectionPtr&, Buffer*)> MessageCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallBack;

    TcpConnection(EventLoop *loop, int sockfd, const SockAddr &peerAddr,
                  const SockAddr &localAddr);
    ~TcpConnection();

    void setMessageCallBack(const MessageCallBack &cb) {
        _messageCallBack = cb;
    }

private:
    /// client与server连接的状态标志
    enum StateE { cDisconnected, cConnecting,  cConnected, cDisconnecting};

    //void handleRead(Time recviveTime);
    void handleRead();
    void handleWrite();
    void handleError();
    void handleClose();

    void shutdownInLoop();

    EventLoop *_loop;
    const SockAddr _peerAddr;
    const SockAddr _localAddr;
    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Chnnel> _chnnel;

    Buffer _inputBuffer;                        /// 用于数据的读
    Buffer _outputBuffer;                       /// 用于数据的写

    MessageCallBack _messageCallBack;
    WriteCompleteCallBack _writeCompleteCallBack;

    StateE _state;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}

#endif