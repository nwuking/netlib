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
#include "netlib/net/CallBack.h"

#include <memory>
#include <functional>
#include <string>

struct tcp_info;

namespace netlib
{

//struct tcp_info;

class EventLoop;
class Chnnel;
class Socket;

class TcpConnection : NonCopyAble, 
                      public std::enable_shared_from_this<TcpConnection>
{
public:
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
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);

    void startRead();
    void stopRead();
    bool isReading() const {
        return _reading;
    }

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

    void setHightWaterMarkCallBack(const HightWaterMarkCallBack &cb, size_t hightWaterMark) {
        _hightWaterMarkCallBack = cb;
        _hightWaterMark = hightWaterMark;
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

    Buffer* inputBuffer() {
        return &_inputBuffer;
    }

    Buffer* outputBuffer() {
        return &_outputBuffer;
    }

private:
    /// client与server连接的状态标志
    enum StateE { cDisconnected, cConnecting,  cConnected, cDisconnecting};

    void handleRead(Time recviveTime);
    //void handleRead();
    void handleWrite();
    void handleError();
    void handleClose();

    void sendInLoop(const std::string &message);
    void sendInLoop(const void *message, size_t len);

    void shutdownInLoop();
    void forceCloseInLoop();

    void setState(StateE s) {
        _state = s;
    }

    const char* stateToString() const;

    void startReadInLoop();
    void stopReadInLoop();

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

    size_t _hightWaterMark;
    HightWaterMarkCallBack _hightWaterMarkCallBack;

    StateE _state;

    bool _reading;
};

//typedef TcpConnection::TcpConnectionPtr TcpConnectionPtr;

}

#endif