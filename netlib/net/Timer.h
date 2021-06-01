#ifndef TIMER_H_
#define TIMER_H_

#include "netlib/base/Time.h"
#include "netlib/base/noncopyable.h"

#include <functional>
#include <atomic>

namespace netlib
{


class Timer : NonCopyAble
{
public:
    typedef std::function<void()> TimerCallBack;

    Timer(TimerCallBack cb, Time when, double interval);
    ~Timer();

    int64_t sequence() const {
        return _sequence;
    }

    void run() {
        _callback();
    }

    Time expiration() const {
        return _expiration;
    }

    bool repeat() const {
        return _repeat;
    }

    void restart(Time now);

private:
    const TimerCallBack _callback;
    Time _expiration;
    const double _interval;
    const bool _repeat;
    const int64_t _sequence;

    static std::atomic<int64_t> INDEX;
};


}


#endif