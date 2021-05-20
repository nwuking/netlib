#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "./TcpConnection.h"

namespace netlib
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient
{
public:
    TcpClient(EventLoop *loop, const SockAddr &addr, const std::string &name);
    ~TcpClient();

    void connect();

private:
    EventLoop *_loop;
    const std::string _name;
    ConnectorPtr _connector;
    bool _connect;
};


}

#endif