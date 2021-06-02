#ifndef TIME_H_
#define TIME_H_

/*
 *  本文件用来表示时间
 */

#include <stdint.h>

#include <string>

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

    bool vaild() const {
        return _microSecondsSinceEpoch > 0;
    }

    std::string toString() const;

    std::string toFormattedString(bool showMicroSeconds = true) const;

    static Time now();

    static Time invaild() {
        return Time();
    }

    static Time fromUnixTime(time_t t) {
        return fromUnixTime(t, 0);
    }

    static Time fromUnixTime(time_t t, int microseconds) {
        return Time(static_cast<int64_t>(t)*cMicroSecondsPerSecond + microseconds);
    }
private:
    int64_t _microSecondsSinceEpoch;
};

inline bool operator<(Time l, Time r) {
    return l.getMicroSecondsSinceEpoch() < r.getMicroSecondsSinceEpoch();
}

inline Time addTime(Time now, double seconds) {
    int64_t delta = static_cast<int64_t>(seconds*Time::cMicroSecondsPerSecond);
    return Time(delta+now.getMicroSecondsSinceEpoch());
}

inline double timeDifference(Time hight, Time low) {
    int64_t diff = hight.getMicroSecondsSinceEpoch() - low.getMicroSecondsSinceEpoch();
    return static_cast<double>(diff) / Time::cMicroSecondsPerSecond;
}

}


#endif