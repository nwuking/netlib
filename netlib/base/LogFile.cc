#include "netlib/base/LogFile.h"
#include "netlib/base/ProcessInfo.h"
#include "netlib/base/FileUtil.h"

#include <assert.h>
#include <time.h>
#include <stdio.h>

using namespace netlib;

LogFile::LogFile(const std::string &basename, off_t rollSize,
                int flushInterval, int checkEveryN)
    : _basename(basename),
      _rollSize(rollSize),
      _flushInterval(flushInterval),
      _checkEveryN(checkEveryN),
      _count(0),
      _lastRoll(0),
      _lastFlush(0),
      _startOfPeriod(0)
{
    assert(_basename.find('/') == std::string::npos);
    rollFile();
}

LogFile::~LogFile() = default;

void LogFile::append(const char *msg, int len) {
    _file->append(msg, len);

    if(_file->writtenBytes() > _rollSize) {
        rollFile();
    }
    else {
        ++_count;
        if(_count >= _checkEveryN) {
            _count = 0;
            time_t now = ::time(NULL);
            time_t thisPeriod = now / _cRollPerSeconds * _cRollPerSeconds;

            if(thisPeriod != _startOfPeriod) {
                rollFile();
            }
            else if(now - _lastFlush > _flushInterval) {
                _lastFlush = now;
                _file->flush();
            }
        }
    }
}

void LogFile::flush() {
    _file->flush();
}

bool LogFile::rollFile() {
    /// 自动归档功能
    time_t now = 0;
    std::string filename = getLogFileName(_basename, &now);
    time_t start = now / _cRollPerSeconds * _cRollPerSeconds;
    if(now > _lastRoll) {
        _lastRoll = now;
        _lastFlush = now;
        _startOfPeriod = start;

        _file.reset(new netlib::AppendFile(filename));
        return true;
    }

    return false;
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