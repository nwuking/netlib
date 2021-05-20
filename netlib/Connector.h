#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include "./SockAddr.h"

#include <memory>
#include <functional>

namespace netlib
{

class EventLoop;
class Chnnel;

class Connector
{
public:
    Connector(EventLoop *loop, const SockAddr &addr);
    ~Connector();

    void start();

    const SockAddr& serverAddress() const {
        return _addr;
    }

private:
    enum States {cDisconnected, cConnecting, cConnected};

    void startInLoop();
    void connect();

    EventLoop *_loop;
    SockAddr _addr;
    bool _connect;
    States _state;
    std::unique_ptr<Chnnel> _chnnel;
};

}

#endif