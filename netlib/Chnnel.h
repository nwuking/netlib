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
    //typedef std::function<void(Time)> ReadEventCallBack;

    Chnnel(EventLoop *loop, int fd);
    ~Chnnel();

    void handleEvent();

    void remove();

    void tie(const std::shared_ptr<void> &obj);

    std::string reventsToString() const;
    std::string eventsToString() const;

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

    bool isWriting() {
        /// 是否注册了写事件
        return _events & cWriteEvent;
    }

    bool isNoneEvent() {
        return _events == cNonEvent;
    }

    void disableWriting() {
        _events &= ~cWriteEvent;
        update();
    }

    void diableAll() {
        _events = cNonEvent;
        update();
    }

    EventLoop* owerLoop() {
        return _ownLoop;
    }

    int flag() const {
        return _flag;
    }

    void setFlag(int flag) {
        _flag = flag;
    }

    void enableReading() {
        _events |= cReadEvent;
        update();
    }

    void enableWriting() {
        _events |= cWriteEvent;
        update();
    }

private:
    void update();
    void handleEventWithGuard();
    static std::string eventsToString(int fd, int events);

    static const int cNonEvent;
    static const int cReadEvent;
    static const int cWriteEvent;

    EventLoop *_ownLoop;
    int _fd;
    int _events;
    int _revevts;
    int _flag;                                      /// use by Epoller
    bool _addedToLoop;                              /// 标志当前的Chnnel是否注册到EventLoop中
    bool _eventHanding;
    bool _tied;
    bool _logHup;

    std::weak_ptr<void> _tie;

    EventCallBack _errorCallBack;
    EventCallBack _writeCallBack;
    EventCallBack _readCallBack;
    EventCallBack _closeCallBack;
};

}


#endif