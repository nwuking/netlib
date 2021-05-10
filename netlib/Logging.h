#ifndef LOGGING_H_
#define LOGGING_H_

#include "./LogStream.h"
#include "./Time.h"

#include <string.h>

namespace netlib
{

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
        FATAL
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
    ~Logger();
private:

    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;
        Impl(const SourceFile &filenmae, int line, LogLevel level);
        void formatTime();

        SourceFile _basename;
        int _line;
        LogLevel _level;
        LogStream _stream;
        Time _time;
    };

    Impl _impl;

};


}


#endif