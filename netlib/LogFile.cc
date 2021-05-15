#include "./LogFile.h"
#include "./ProcessInfo.h"

#include <assert.h>
#include <time.h>

using namespace netlib;

LogFile::LogFile(const std::string &basename, off_t rollSize,
                int flushInterval, int checkEveryN)
    : _basename(basename),
      _rollSize(rollSize),
      _flushInterval(flushInterval),
      _checkEveryN(checkEveryN)
{
    assert(_basename.find('/') == std::string::npos);
    rollFile();
}

bool LogFile::rollFile() {
    /// 自动归档功能
    time_t now = 0;
    std::string filename = getLogFileName(_basename, &now);
    //
}

std::string LogFile::getLogFileName(const std::string &basename, time_t *t) {
    std::string filename;
    filename.reserve(basename.length()+64);
    filename = basename;

    char timebuf[32];
    struct tm _tm;
    *t = ::time(NULL);
    ::gmtime_r(t, &_tm);
    ::strftime(timebuf, sizeof timebuf, ".%y%m%d-%H%M%S.", &_tm);
    filename += timebuf;
    filename += ProcessInfo::hostname();

    char pidbuf[32];
    ::snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::pid());
    filename += pidbuf;

    filename += ".log";
    return filename;
}