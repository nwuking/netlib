#include "netlib/net/EventLoopThread.h"
#include "netlib/net/EventLoop.h"

#include <assert.h>

using namespace netlib;

EventLoopThread::EventLoopThread(const ThreadInitCallBack &cb,
                                 const std::string &name)
    : _thread(std::bind(&EventLoopThread::threadFunc, this), name),
      _callback(cb),
      _exiting(false),
      _mutex(),
      _cond(_mutex),
      _loop(nullptr)
{
}


EventLoopThread::~EventLoopThread() {
    _exiting = true;
    if(_loop != nullptr) {
        _loop->quit();
        _thread.join();
    }
}


EventLoop* EventLoopThread::startLoop() {
    assert(!_thread.started());
    _thread.start();

    EventLoop *loop = nullptr;
    {
        MutexLock lock(_mutex);
        while(_loop == nullptr) {
            _cond.wait();
        }
        loop = _loop;
    }
    return loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;

    if(_callback) {
        _callback(&loop);
    }
    
    {
        MutexLock lock(_mutex);
        _loop = &loop;
        _cond.notify();
    }
    loop.loop();

    MutexLock lock(_mutex);
    _loop = nullptr;
}