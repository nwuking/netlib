#ifndef TCPSERVER_H_
#define TCPSERVER_H_

/// TcpServer-> 用于服务器端的开发
///             服务器端的开发大概为：socket()-> bind()-> listen()-> accept()

#include "./SockAddr.h"
#include "./TcpConnection.h"

#include <memory>

namespace netlib
{

class EventLoop;
class Acceptor;
class EventLoopThreadPool;

class TcpServer
{
public:
    typedef std::function<void(const TcpConnectionPtr&, Buffer*)> MessageCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;

    TcpServer(EventLoop *loop, SockAddr &listenAddr);
    ~TcpServer();

    void setMessageCallBack(const MessageCallBack &cb) {
        _messageCallBack = cb;
    }

    void setWriteCompleteCallBack(const WriteCompleteCallBack &cb) {
        _writeCompleteCallBack = cb;
    }

private:
    void newConnection(int fd, SockAddr &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    EventLoop *_loop;
    SockAddr _listenAddr;

    std::unique_ptr<Acceptor> _acceptor;                    /// 用于监听，等待client的到来，只有一个
    std::shared_ptr<EventLoopThreadPool> _threadPool;           /// 线程池，一个线程一个EventLoop

    /// 由用户自定义
    MessageCallBack _messageCallBack;
    WriteCompleteCallBack _writeCompleteCallBack;
    ConnectionCallBack _connectionCallBack;
};

}

#endif