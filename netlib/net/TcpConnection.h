#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

/*
 *  封装accept()返回的描述符和客户端的地址结构
 *  表示server和client的一条链接
 */

#include "netlib/net/SockAddr.h"
#include "netlib/base/Time.h"
#include "netlib/net/Buffer.h"
#include "netlib/base/noncopyable.h"

#include <memory>
#include <functional>
#include <string>

namespace netlib
{

struct tcp_info;

class EventLoop;
class Chnnel;
class Socket;

class TcpConnection : NonCopyAble, 
                      public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr&, Buffer*)> MessageCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;

    TcpConnection(EventLoop *loop, int sockfd, const SockAddr &peerAddr,
                  const SockAddr &localAddr, const std::string &name);
    ~TcpConnection();

    void conncetDestoryed();
    void connectEstablished();
    void shutdown();

    bool getTcpInfo(struct tcp_info*) const;
    std::string getTcpInfoString() const;

    void send(const void *meaasge, int len);
    void send(const std::string &message);
    void send(Buffer *message);

    void forceClose();

    void setMessageCallBack(const MessageCallBack &cb) {
        _messageCallBack = cb;
    }

    void setWriteCompleteCallBack(const WriteCompleteCallBack &cb) {
        _writeCompleteCallBack = cb;
    }

    void setCloseCallBack(const CloseCallBack &cb) {
        _closeCallBack = cb;
    }

    void setConnectionCallBack(const ConnectionCallBack &cb) {
        _connectionCallBack = cb;
    }

    EventLoop* getLoop() const {
        return _loop;
    }

    const std::string& name() const {
        return _name;
    }

    const SockAddr& localAddress() const {
        return _localAddr;
    }

    const SockAddr& peerAddress() const {
        return _peerAddr;
    }

    bool connected() const {
        return _state == cConnected;
    }

    bool disconnected() const {
        return _state == cDisconnected;
    }

    

private:
    /// client与server连接的状态标志
    enum StateE { cDisconnected, cConnecting,  cConnected, cDisconnecting};

    //void handleRead(Time recviveTime);
    void handleRead();
    void handleWrite();
    void handleError();
    void handleClose();

    void sendInLoop(const std::string &message);
    void sendInLoop(const void *message, int len);

    void shutdownInLoop();
    void forceCloseInLoop();

    void setState(StateE s) {
        _state = s;
    }

    const char* stateToString() const;

    EventLoop *_loop;
    const SockAddr _peerAddr;
    const SockAddr _localAddr;
    const std::string _name;
    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Chnnel> _chnnel;

    Buffer _inputBuffer;                        /// 用于数据的读
    Buffer _outputBuffer;                       /// 用于数据的写

    MessageCallBack _messageCallBack;
    WriteCompleteCallBack _writeCompleteCallBack;
    CloseCallBack _closeCallBack;
    ConnectionCallBack _connectionCallBack;

    StateE _state;
};

typedef TcpConnection::TcpConnectionPtr TcpConnectionPtr;

}

#endif