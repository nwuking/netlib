#include "./Timer.h"

using namespace netlib;

std::atomic<int64_t> Timer::INDEX(0);

Timer::Timer(TimerCallBack cb, Time when, double interval)
    : _callback(std::move(cb)),
      _expiration(when),
      _interval(interval),
      _repeat(interval > 0.0),
      _sequence(++INDEX)
{
}

void Timer::restart(Time now) {
    if(_repeat) {
        _expiration = addTime(now, _interval);
    }
    else {
        _expiration = Time::invaild();
    }
}