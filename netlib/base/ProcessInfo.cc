#include "netlib/base/ProcessInfo.h"

//#include <limits.h>
#include <unistd.h>

using namespace netlib;

std::string ProcessInfo::hostname() {
    /// 获取主机名
    /// HOST_NAME_MAX 64
    /// _POSIX_HOST_NAME_MAX 255

    char buf[256];
    if(::gethostname(buf, sizeof buf) == 0) {
        buf[sizeof(buf)-1] = '\0';
        return buf;
    }
    else {
        return "unknwonhost";
    }
}

pid_t ProcessInfo::pid() {
    return ::getpid();
}