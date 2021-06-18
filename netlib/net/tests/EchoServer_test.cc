#include "netlib/net/TcpServer.h"
#include "netlib/base/Logging.h"
#include "netlib/net/EventLoop.h"
#include "netlib/net/SockAddr.h"

#include <unistd.h>
#include <iostream>

int numThreads = 0;

int count = 0;

class EchoServer
{
public:
    EchoServer(netlib::EventLoop *loop, const netlib::SockAddr &addr)
        : _loop(loop),
          _server(loop, addr, "echo")
    {
        _server.setConnectionCallBack(
            std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
        _server.setMessageCallBack(
            std::bind(&EchoServer::onMessage, this, std::placeholders::_1,
                        std::placeholders::_2, std::placeholders::_3));
        _server.setThreadNums(numThreads);
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

        con->send("Hello\n");
        std::cout << "Con" << count++ << "\n";
    }

    void onMessage(const netlib::TcpConnectionPtr &con, netlib::Buffer *buf, netlib::Time time) {
        std::string msg(buf->retrieveAllAsString());
        LOG_TRACE << con->name() << " recv " << msg.size() 
                  << " bytes by at " << time.toString();
        if(msg == "exit\n") {
            con->send("bye\n");
            con->shutdown();
        }
        if(msg == "quit\n") {
            _loop->quit();
        }
        con->send(msg);
        std::cout << "Mes" << count++ << "\n";
    }

    netlib::EventLoop *_loop;
    netlib::TcpServer _server;
};

int main(int argc, char **argv)
{
    LOG_INFO << "pid: " << getpid() << " tid: " << netlib::CurrentThread::tid();
    LOG_INFO << "sizeof TcpConnection: " << sizeof(netlib::TcpConnection);
    if(argc > 1) {
        numThreads = ::atoi(argv[1]);
    }
    //bool ipv6 = argc > 2;
    netlib::EventLoop loop;
    netlib::SockAddr addr(5000, false);
    EchoServer server(&loop, addr);

    server.start();

    loop.loop();
    return 0;
}