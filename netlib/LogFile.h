#ifndef LOGFILE_H_
#define LOGFILE_H_

#include "./noncopyable.h"

#include <string>
#include <memory>

namespace netlib
{

class AppendFile;

class LogFile : public NonCopyAble
{
public:
    LogFile(const std::string &basename, off_t rollSize, 
            int flushInterval = 3, int checkEveryN = 1024);
    ~LogFile();

    void append(const char *msg, int len);

    void flush();

    bool rollFile();

private:

    static std::string getLogFileName(const std::string &basename, time_t *t);

    const std::string _basename;
    const off_t _rollSize;
    const int _flushInterval;
    const int _checkEveryN;

    int _count;

    time_t _lastRoll;
    time_t _lastFlush;
    time_t _startOfPeriod;

    std::unique_ptr<netlib::AppendFile> _file;

    static const int _cRollPerSeconds = 60*60*24;
};


}

#endif