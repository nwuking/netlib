#ifndef TIME_H_
#define TIME_H_

/*
 *  本文件用来表示时间
 */

#include <stdint.h>

namespace netlib
{

class Time
{
public:
    static const int cMicroSecondsPerSecond = 1000 * 1000;

    Time() : _microSecondsSinceEpoch(0) {}
    explicit Time(int64_t microSeconds) : _microSecondsSinceEpoch(microSeconds) {}
    Time(int64_t second, int64_t microSeconds) {
        _microSecondsSinceEpoch = microSeconds + second * cMicroSecondsPerSecond;
    }
    /// ~Time();                默认析构

    int64_t getMicroSecondsSinceEpoch() {
        return _microSecondsSinceEpoch;
    }

    int64_t getSecondSinceEpoch() {
        return _microSecondsSinceEpoch / cMicroSecondsPerSecond;
    }

    static Time now();
private:
    int64_t _microSecondsSinceEpoch;
};

}


#endif