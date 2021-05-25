#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include "./SockAddr.h"

#include <memory>
#include <functional>

namespace netlib
{

class EventLoop;
class Chnnel;

class Connector : public std::enable_shared_from_this<Connector>
{
public:
    typedef std::function<void(int fd)> NewConnectionCallBack;

    Connector(EventLoop *loop, const SockAddr &addr);
    ~Connector();

    void start();

    void restart();

    void stop();

    const SockAddr& serverAddress() const {
        return _addr;
    }

    void setNewConnectionCallBack(const NewConnectionCallBack &cb) {
        _newConnectionCallBack = cb;
    }

    const SockAddr& serverAddress() const {
        return _addr;
    }

private:
    enum States {cDisconnected, cConnecting, cConnected};
    static const int cInitRetryDelayMs = 500;
    static const int cMaxRetryDelayMs = 30*1000;

    void setState(States s) {_state = s;}
    void startInLoop();
    void stopInLoop();
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

    int _retryDelayMs;
};

}

#endif