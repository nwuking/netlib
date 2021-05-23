#ifndef TIMERQUEUE_H_
#define TIMERQUEUE_H_

#include "./Chnnel.h"
#include "./Timer.h"
#include "./Time.h"

namespace netlib
{

class EventLoop;
class TimerId;

class TimerQueue
{
public:
    typedef Timer::TimerCallBack TimerCallBack;

    TimerQueue(EventLoop *loop);
    ~TimerQueue();

    TimerId addTimer(TimerCallBack cb, Time when, double interval);

    void cancleTimer(TimerId timerId);
private:
    EventLoop *_loop;
    
};


}

#endif