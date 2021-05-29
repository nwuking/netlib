#ifndef TCPSERVER_H_
#define TCPSERVER_H_

/// TcpServer-> 用于服务器端的开发
///             服务器端的开发大概为：socket()-> bind()-> listen()-> accept()

#include "./SockAddr.h"
#include "./TcpConnection.h"
#include "./noncopyable.h"

#include <memory>
#include <map>

namespace netlib
{

class EventLoop;
class Acceptor;
class EventLoopThreadPool;

class TcpServer : NonCopyAble
{
public:
    typedef std::function<void(const TcpConnectionPtr&, Buffer*)> MessageCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;
    typedef std::function<void(EventLoop*)> ThreadInitCallBack;

    enum Option {
        /// 是否开启端口复用选项
        cNoReusePort,
        cReusePort,
    };

    TcpServer(EventLoop *loop, SockAddr &listenAddr, const std::string &name,
                Option option = cNoReusePort);
    ~TcpServer();

    void start();

    void setThreadNums(int nums);

    const std::string& ipPort() const {
        return _ipPort;
    }

    EventLoop* getLoop() const {
        return _loop;
    }

    void setMessageCallBack(const MessageCallBack &cb) {
        _messageCallBack = cb;
    }

    void setWriteCompleteCallBack(const WriteCompleteCallBack &cb) {
        _writeCompleteCallBack = cb;
    }

    void setConnectionCallBack(const ConnectionCallBack &cb) {
        _connectionCallBack = cb;
    }

    void setThreadInitCallBack(const ThreadInitCallBack &cb) {
        _threadInitCallBack = cb;
    }

private:
    void newConnection(int fd, const SockAddr &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop *_loop;
    const std::string _ipPort;
    SockAddr _listenAddr;

    std::unique_ptr<Acceptor> _acceptor;                    /// 用于监听，等待client的到来，只有一个
    std::shared_ptr<EventLoopThreadPool> _threadPool;           /// 线程池，一个线程一个EventLoop

    /// 由用户自定义
    MessageCallBack _messageCallBack;
    WriteCompleteCallBack _writeCompleteCallBack;
    ConnectionCallBack _connectionCallBack;
    ThreadInitCallBack _threadInitCallBack;

    const std::string _name;
    int _nextConnId;
    ConnectionMap _connections;                             /// 记录管理所生成的连接对象

    bool _started;
};

}

#endif