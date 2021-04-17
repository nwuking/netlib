#include "./Time.h"

#include <sys/time.h>

using namespace netlib;

Time Time::now() {
    /// 获取当前的时间
    struct timeval tv;
    ::gettimeofday(&tv, nullptr);
    int64_t seconds = tv.tv_sec;
    return Time(seconds, tv.tv_usec);
}