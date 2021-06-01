#ifndef TIMERID_H_
#define TIMERID_H_

#include <stdlib.h>

namespace netlib
{

class Timer;

class TimerId
{
public:
    TimerId()
        : _timer(nullptr),
          _sequence(0)
    {}

    TimerId(Timer *timer, int64_t seq)
        : _timer(timer),
          _sequence(seq)
    {
    }

    friend class TimerQueue;

private:
    Timer *_timer;
    int64_t _sequence;
};


}


#endif