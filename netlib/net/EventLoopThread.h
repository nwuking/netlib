#ifndef EVENTLOOPTHREAD_H_
#define EVENTLOOPTHREAD_H_

/*
 *  one loop peer thread
 */

#include "netlib/base/Thread.h"
#include "netlib/base/Mutex.h"
#include "netlib/base/Condition.h"
#include "netlib/net/EventLoop.h"

namespace netlib
{

//class EventLoop;

class EventLoopThread : NonCopyAble
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallBack;

    EventLoopThread(const ThreadInitCallBack &cb = ThreadInitCallBack(),
                    const std::string &name = std::string());
    ~EventLoopThread() ;
    
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