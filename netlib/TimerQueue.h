#ifndef TIMERQUEUE_H_
#define TIMERQUEUE_H_

#include "./Chnnel.h"
#include "./Timer.h"
#include "./Time.h"

#include <set>
#include <vector>

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
    typedef std::pair<Time, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer*, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void handleRead();
    std::vector<Entry> getExpired(Time &now);
    void reset(const std::vector<Entry> &expired, Time &now);

    bool insert(Timer *timer);
    void addTimerInLoop(Timer *timer);
    void cancleInLoop(TimerId timerId);

    EventLoop *_loop;
    const int _timerFd;
    Chnnel _timerFdChnnel;
    TimerList _timers;

    bool _callingExpiredTimer;
    ActiveTimerSet _activeTimers;
    ActiveTimerSet _canllingTimers;
};


}

#endif