#include "./FileUtil.h"

#include <assert.h>
#include <string.h>

using namespace netlib;

AppendFile::AppendFile(std::string filename)
    : _fp(::fopen(filename.c_str(), "ae")),
      _writtenBytes(0)
{
    assert(_fp);
    ::setbuffer(_fp, _buf, sizeof _buf);
}

AppendFile::~AppendFile() {
    if(_fp) {
        ::fclose(_fp);
    }
}

void AppendFile::append(const char *msg, int len) {
    size_t written = 0;

    while(written != len) {
        size_t remain = len - written;
        size_t n = write(msg+written, remain);

        if(n != remain) {
            int err = ::ferror(_fp);
            if(err) {
                /// err非0,_fp流出现错误
                fprintf(stderr, "AppendFile::apend() failed %s\n", ::strerror(err));
                break;
            }
        }

        written += n;
    }

    _writtenBytes += written;
}

size_t AppendFile::write(const char *msg, int len) {
    return ::fwrite_unlocked(msg, 1, len, _fp);
}

void AppendFile::flush() {
    ::fflush(_fp);
}