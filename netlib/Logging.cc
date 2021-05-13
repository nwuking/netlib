#include "./Logging.h"
#include "./TimeZone.h"
#include "./CurrentThread.h"

#include <assert.h>

namespace netlib
{

__thread time_t t_lastSecond;
__thread char t_time[64];

class T
{
public:
    T(const char *str, unsigned len) 
        : _str(str), _len(len)
    {
        assert(::strlen(str) == _len);
    }

    const char *_str;
    const unsigned _len;
};

inline LogStream& operator<<(LogStream &s, T v) {
    s.append(v._str, v._len);
    return s;
}

TimeZone g_logTimeZone;

}


using namespace netlib;

Logger::Impl::Impl(const SourceFile &filename, int line, LogLevel level)
    :   _basename(filename),
        _line(line),
        _level(level),
        _stream(),
        _time(Time::now())
{
    formatTime();
    CurrentThread::tid();
    //
}

void Logger::Impl::formatTime() {
    /// 一条日志的开头固定的部分
    /// 时间
    int64_t mincroSecondsSinceEpoch = _time.getMicroSecondsSinceEpoch();
    time_t seconds = static_cast<time_t>(mincroSecondsSinceEpoch / Time::cMicroSecondsPerSecond);
    int mincroSeconds = static_cast<int>(mincroSecondsSinceEpoch % Time::cMicroSecondsPerSecond);

    if(seconds != t_lastSecond) {
        t_lastSecond = seconds;
        struct tm tm_time;
        if(netlib::g_logTimeZone.vaild()) {
            tm_time = g_logTimeZone.toLocalTime(seconds);
        }
        else {
            ::gmtime_r(&seconds, &tm_time);
        }

        int len = ::snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
            tm_time.tm_year+1900, tm_time.tm_mon+1, tm_time.tm_mday, 
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        assert(len == 17);
        (void)len;
    }

    if(netlib::g_logTimeZone.vaild()) {
        Fmt us(".%06d", mincroSeconds);
        assert(us.length() == 8);
        _stream << T(t_time, 17) << T(us.data(), 8);
    }
    else {
        Fmt us("%.06dZ", mincroSeconds);
        assert(us.length() == 9);
        _stream << T(t_time, 17) << T(us.data(), 9);
    }
}

