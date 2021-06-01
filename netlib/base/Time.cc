#include "netlib/base/Time.h"

#include <sys/time.h>
#include <inttypes.h>

using namespace netlib;

std::string Time::toString() const {
    char buf[32] = {0};
    int64_t seconds = _microSecondsSinceEpoch / cMicroSecondsPerSecond;
    int64_t microseconds = _microSecondsSinceEpoch % cMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

std::string Time::toFormattedString(bool showMicroSeconds) const {
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(_microSecondsSinceEpoch / cMicroSecondsPerSecond);
    struct tm t;
    ::gmtime_r(&seconds, &t);

    if(showMicroSeconds) {
        int microSeconds = static_cast<int>(_microSecondsSinceEpoch % cMicroSecondsPerSecond);
        snprintf(buf, sizeof buf, "%4d%2d%2d %2d:%2d:%2d.%6d", 
                t.tm_year+1900, t.tm_mon+1, t.tm_mday,
                t.tm_hour, t.tm_min, t.tm_sec, microSeconds);
    }
    else {
        snprintf(buf, sizeof buf, "%4d%2d%2d %2d:%2d:%2d", 
                t.tm_year+1900, t.tm_mon+1, t.tm_mday,
                t.tm_hour, t.tm_min, t.tm_sec);
    }

    return buf;
}

Time Time::now() {
    /// 获取当前的时间
    struct timeval tv;
    ::gettimeofday(&tv, nullptr);
    int64_t seconds = tv.tv_sec;
    return Time(seconds, tv.tv_usec);
}