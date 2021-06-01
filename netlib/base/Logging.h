#ifndef LOGGING_H_
#define LOGGING_H_

#include "netlib/base/LogStream.h"
#include "netlib/base/Time.h"

#include <string.h>

namespace netlib
{

class TimeZone;

class Logger
{
public:
    enum LogLevel
    {
        /// 日志等级
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,                                 //  
    };

    /// 运行中计算一个文件的basename
    class SourceFile
    {
    public:
        template<int N>
        SourceFile(const char (&arr)[N])
         : _data(arr),
           _size(N-1)
        {
            /// __FILE__ : 一个保存文件名的数组
            const char *pos = ::strrchr(_data, '/');
            if(pos) {
                _data = pos + 1;
                _size -= static_cast<int>(_data - arr); 
            }
        }

        explicit SourceFile(const char *filename)
            : _data(filename)
        {
            const char *pos = ::strrchr(_data, '/');
            if(pos) {
                _data = pos + 1;
            }
            _size = static_cast<int>(::strlen(_data));
        }

        const char *_data;
        int _size;
    };

    Logger(SourceFile filename, int line);
    Logger(SourceFile filename, int line, LogLevel level);
    Logger(SourceFile filename, int line, LogLevel level, const char *func);
    Logger(SourceFile filename, int line, bool toAbort);
    ~Logger();

    LogStream& stream() {
        return _impl._stream;
    }

    typedef void (*OutputFunc)(const char *msg, int len);
    typedef void (*FlushFunc)();

    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
    static void setTimeZone(const TimeZone &tz);
    static void setLogLevel(LogLevel level);
    static LogLevel logLevel();

private:

    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;
        Impl(const SourceFile &filenmae, int line, LogLevel level);
        void formatTime();
        void finish();

        SourceFile _basename;
        int _line;
        LogLevel _level;
        LogStream _stream;
        Time _time;
    };

    Impl _impl;

};

extern Logger::LogLevel g_logLevel;
 
inline Logger::LogLevel Logger::logLevel() {
    return g_logLevel;
}


/// 宏定义一些对象(与Logger关联)，供外部使用

#define LOG_TRACE if(netlib::Logger::logLevel() <= netlib::Logger::TRACE) \
    netlib::Logger(__FILE__, __LINE__, netlib::Logger::TRACE, __func__).stream()

#define LOG_DEBUG if(netlib::Logger::logLevel() <= netlib::Logger::DEBUG) \
    netlib::Logger(__FILE__, __LINE__, netlib::Logger::DEBUG, __func__).stream()

#define LOG_INFO if(netlib::Logger::logLevel() << netlib::Logger::INFO) \
    netlib::Logger(__FILE__, __LINE__).stream()

#define LOG_WARN netlib::Logger(__FILE__, __LINE__, netlib::Logger::WARN).stream()

#define LOG_ERROR netlib::Logger(__FILE__, __LINE__, netlib::Logger::ERROR).stream()

#define LOG_FATAL netlib::Logger(__FILE__, __LINE__, netlib::Logger::FATAL).stream()

#define LOG_SYSERR netlib::Logger(__FILE__, __LINE__, false).stream()

#define LOG_SYSFATAL netlib::Logger(__FILE__, __LINE__, true).stream()

}


#endif