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
    typedef std::function<void(int fd)> NewConnectionCallBack;

    Connector(EventLoop *loop, const SockAddr &addr);
    ~Connector();

    void start();

    const SockAddr& serverAddress() const {
        return _addr;
    }

    void setNewConnectionCallBack(const NewConnectionCallBack &cb) {
        _newConnectionCallBack = cb;
    }

private:
    enum States {cDisconnected, cConnecting, cConnected};

    void setState(States s) {_state = s;}
    void startInLoop();
    void connect();
    void connecting(int sockFd);
    void retry(int fd);
    void handleWrite();
    void handleError();
    int removeAndResetChnnel();
    void resetChnnel();

    EventLoop *_loop;
    SockAddr _addr;
    bool _connect;
    States _state;
    std::unique_ptr<Chnnel> _chnnel;

    NewConnectionCallBack _newConnectionCallBack;
};

}

#endif