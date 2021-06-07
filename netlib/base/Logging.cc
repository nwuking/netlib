#include "netlib/base/Logging.h"
#include "netlib/base/TimeZone.h"
#include "netlib/base/CurrentThread.h"
#include "netlib/base/Time.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace netlib
{

__thread time_t t_lastSecond;
__thread char t_time[64];

Logger::LogLevel initLogLevel() {
    if(::getenv("NETLIB_LOG_TRACE"))
        return Logger::TRACE;
    else if(::getenv("NETLIB_LOG_DEBUG"))
        return Logger::DEBUG;
    else 
        return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
    /// 为了对齐好看，方便，  添加空格字符：6个字符
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};

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

inline LogStream& operator<<(LogStream &s, const Logger::SourceFile &file) {
    s.append(file._data, file._size);
    return s;
}

void defaultOutput(const char *msg, int len) {
    /// 默认的日志输出函数
    size_t n = ::fwrite(msg, 1, len, stdout);
    (void)n;
}

void defaultFlush() {
    ::fflush(stdout);
}

Logger::OutputFunc g_outPut = defaultOutput;
Logger::FlushFunc g_Flush = defaultFlush;

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
    /// 固定的日志开头部分
    formatTime();
    CurrentThread::tid();
    _stream << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
    _stream << T(netlib::LogLevelName[_level], 6);
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
        Fmt us(".%06d ", mincroSeconds);
        assert(us.length() == 8);
        _stream << T(t_time, 17) << T(us.data(), 8);
    }
    else {
        Fmt us(".%06dZ ", mincroSeconds);
        assert(us.length() == 9);
        _stream << T(t_time, 17) << T(us.data(), 9);
    }
}

void Logger::Impl::finish() {
    /// 给日志尾部添加固定格式数据
    _stream << " - " << _basename << ":" << _line << "\n";
}

Logger::Logger(SourceFile filename, int line)
    : _impl(filename, line, INFO)
{
}

Logger::Logger(SourceFile filename, int line, LogLevel level)
    : _impl(filename, line, level)
{
}

Logger::Logger(SourceFile filename, int line, LogLevel level, const char *func)
    : _impl(filename, line, level)
{
    _impl._stream << func << ' ';
}

Logger::Logger(SourceFile filename, int line, bool toAbort)
    : _impl(filename, line, toAbort?FATAL:ERROR)
{
}

Logger::~Logger() {
    _impl.finish();

    /// 通过一个全局的变量将日志传到缓冲区
    const LogStream::Buffer &buf(stream().buffer());
    g_outPut(buf.data(), buf.length());

    if(_impl._level == Logger::FATAL) {
        g_Flush();
        ::abort();
    }
}

void Logger::setOutput(OutputFunc fun) {
    g_outPut = fun;
}

void Logger::setFlush(FlushFunc fun) {
    g_Flush = fun;
}

void Logger::setTimeZone(const TimeZone &tz) {
    g_logTimeZone = tz;
}

void Logger::setLogLevel(Logger::LogLevel level) {
    g_logLevel = level;
}

