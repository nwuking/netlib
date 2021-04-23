#ifndef EVENTLOOPTHREAD_H_
#define EVENTLOOPTHREAD_H_

/*
 *  one loop peer thread
 */

#include "./Thread.h"
#include "./Mutex.h"
#include "./Condition.h"

namespace netlib
{

class EventLoop;

class EventLoopThread
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallBack;

    EventLoopThread(const ThreadInitCallBack &cb = ThreadInitCallBack());
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

    Thread _thread;
    ThreadInitCallBack _callback;
    bool _exiting;
    Mutex _mutex;
    Condition _cond;
    EventLoop *_loop;
};

}

#endif