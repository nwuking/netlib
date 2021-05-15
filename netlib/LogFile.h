#ifndef LOGFILE_H_
#define LOGFILE_H_

#include <string>

namespace netlib
{

class LogFile
{
public:
    LogFile(const std::string &basename, off_t rollSize, 
            int flushInterval = 3, int checkEveryN = 1024);
    ~LogFile();

    bool rollFile();

private:

    static std::string getLogFileName(const std::string &basename, time_t *t);

    const std::string _basename;
    const off_t _rollSize;
    const int _flushInterval;
    const int _checkEveryN;
};


}

#endif