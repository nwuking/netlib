#include "netlib/net/TcpServer.h"
#include "netlib/base/Logging.h"
#include "netlib/net/EventLoop.h"
#include "netlib/net/SockAddr.h"


class EchoServer
{
public:
    EchoServer(netlib::EventLoop *loop, const netlib::SockAddr &addr)
        : _loop(loop),
          _server(loop, addr, "echo")
    {

    }

    void start() {
        _server.start();
    }

private:
    void onConnection(const netlib::TcpConnectionPtr &con) {
        LOG_TRACE << con->peerAddress().toIpPort() << "->"
                  << con->localAddress().toIpPort() << " is "
                  << (con->connected() ? "UP" : "DOWN");
        LOG_INFO << con->getTcpInfoString();

        //
    }

    netlib::EventLoop *_loop;
    netlib::TcpServer _server;
};