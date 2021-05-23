#ifndef TIMER_H_
#define TIMER_H_

#include "./Time.h"

#include <functional>

namespace netlib
{


class Timer
{
public:
    typedef std::function<void()> TimerCallBack;

    Timer(TimerCallBack cb, Time when, double interval);
    ~Timer();

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

    static int64_t INDEX;
};


}


#endif