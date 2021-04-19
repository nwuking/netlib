#ifndef CHNNEL_H_
#define CHNNEL_H_

#include "./EventLoop.h"

#include <functional>

namespace netlib
{

class Chnnel
{
public:
    typedef std::function<void()> EventCallBack;

    Chnnel(EventLoop *loop, int fd);
    ~Chnnel();

    void handleEvent();

    int getFd() const {
        return _fd;
    }
    int getEvents() const {
        return _events;
    }

    void setRevents(int revents) {
        _revevts = revents;
    }

    void setErrorCallBack(EventCallBack cb) {
        /// *************
        _errorCallBack = std::move(cb);
    }
    void setReadCallBack(EventCallBack cb) {
        _readCallBack = std::move(cb);
    }
    void setWriteCallBack(EventCallBack cb) {
        _writeCallBack = std::move(cb);
    }
    void setCloseCallBack(EventCallBack cb) {
        _closeCallBack = std::move(cb);
    }

private:
    EventLoop *_ownLoop;
    int _fd;
    int _events;
    int _revevts;

    EventCallBack _errorCallBack;
    EventCallBack _writeCallBack;
    EventCallBack _readCallBack;
    EventCallBack _closeCallBack;
};

}


#endif