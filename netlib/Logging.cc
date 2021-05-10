#include "./Logging.h"


namespace netlib
{

__thread time_t t_lastSecond;

//TimeZone g_logTimeZone;

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

    }
}

