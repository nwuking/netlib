#include "netlib/net/TcpClient.h"
#include "netlib/base/Logging.h"
#include "netlib/base/Thread.h"
#include "netlib/net/EventLoop.h"
#include "netlib/net/SockAddr.h"

#include <unistd.h>
#include <string>
#include <vector>
#include <memory>
#include <stdio.h>
#include <utility>

int numThreads = 0;
class EchoClient;
std::vector<std::unique_ptr<EchoClient>> clients;
int current = 0;

class EchoClient
{
public:
    EchoClient(netlib::EventLoop *loop, const netlib::SockAddr &addr, const std::string &id)
        : _loop(loop),
          _client(loop, addr, "EchoClient"+id)
    {
        _client.setConnectionCallBack(std::bind(&EchoClient::onConnection,
                                            this, std::placeholders::_1));
        _client.setMessageCallBack(std::bind(&EchoClient::onMessage, this, 
                                        std::placeholders::_1, std::placeholders::_2,
                                        std::placeholders::_3));
    }

    void connect() {
        _client.connect();
    }

private:
    void onConnection(const netlib::TcpConnectionPtr &con) {
        LOG_TRACE << con->localAddress().toIpPort() << " -> "
                  << con->peerAddress().toIpPort() << " is "
                  << (con->connected() ? "UP" : "DOWN");

        if(con->connected()) {
            ++current;
            if(current < clients.size()) {
                clients[current]->connect();
            }
            LOG_INFO << "*** connected " << current;
        }

        con->send("word\n");
    }

    void onMessage(const netlib::TcpConnectionPtr &con, netlib::Buffer *buf, netlib::Time time) {
        std::string msg(buf->retrieveAllAsString());
        LOG_TRACE << con->name() << " recv " << msg.size() << " bytes at " << time.toString();
        if(msg == "exit\n") {
            con->send("bye\n");
            con->shutdown();
        }
        else if(msg == "shutsown\n") {
            _loop->quit();
        }
        else {
            con->send(msg);
        }
    }

    netlib::EventLoop *_loop;
    netlib::TcpClient _client;
};

int main(int argc, char **argv) 
{
    LOG_INFO << "pid: " << getpid() << " tid: " << netlib::CurrentThread::tid();
    if(argc > 1) {
        netlib::EventLoop loop;
        netlib::SockAddr addr(argv[1], 2000);

        int n = 1;
        if(argc > 2) {
            n = atoi(argv[2]);
        }

        clients.reserve(n);
        for(int i = 0; i < n; ++i) {
            char buf[32];
            snprintf(buf, sizeof buf, "%d", i+1);
            clients.emplace_back(new EchoClient(&loop, addr, buf));
        }
        clients[current]->connect();
        loop.loop();
    }
    return 0;
}