#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include "netlib/base/noncopyable.h"

#include <string>

#include <stdio.h>

namespace netlib
{

class AppendFile : public NonCopyAble
{
public:
    explicit AppendFile(std::string filename);
    ~AppendFile();

    void append(const char *msg, int len);

    void flush();

    off_t writtenBytes() const {
        return _writtenBytes;
    }
private:

    size_t write(const char *msg, int len);

    FILE *_fp;
    off_t _writtenBytes;
    char _buf[64*1024];
};


}

#endif