#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "netlib/net/TcpConnection.h"
#include "netlib/base/Mutex.h"

namespace netlib
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : NonCopyAble
{
public:
    //typedef TcpConnection::MessageCallBack MessageCallBack;
    //typedef TcpConnection::WriteCompleteCallBack WriteCompleteCallBack;
    //typedef TcpConnection::ConnectionCallBack ConnectionCallBack;
    //typedef TcpConnection::CloseCallBack CloseCallBack;

    TcpClient(EventLoop *loop, const SockAddr &serveraddr, const std::string &name);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    void setMessageCallBack(MessageCallBack cb) {
        _messageCallBack = std::move(cb);
    }

    void setWriteCompleteCallBack(WriteCompleteCallBack cb) {
        _writeCompleteCallBack = std::move(cb);
    }

    void setConnectionCallBack(ConnectionCallBack cb) {
        _connectionCallBack = std::move(cb);
    }

    EventLoop* getLoop() const {
        return _loop;
    }

    bool retry() const {
        return _retry;
    }

    void enableRetry() {
        _retry = true;
    }

    const std::string& name() const {
        return _name;
    }

    TcpConnectionPtr connection() {
        MutexLock lock(_mutex);
        return _connection;
    }

private:
    void NewConnection(int fd);
    void removeConnection(const TcpConnectionPtr &conn);

    EventLoop *_loop;
    const std::string _name;
    ConnectorPtr _connector;
    bool _connect;
    bool _retry;
    int _nextConnId;

    MessageCallBack _messageCallBack;
    WriteCompleteCallBack _writeCompleteCallBack;
    ConnectionCallBack _connectionCallBack;

    Mutex _mutex;
    TcpConnectionPtr _connection;
};


}

#endif