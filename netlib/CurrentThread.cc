#include "./CurrentThread.h"

#include <execinfo.h>

namespace netlib
{

namespace CurrentThread
{

__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char *t_threadName = "unknown";

std::string stackTrace(bool demangle) {
    std::string stack;
    const int max_frames = 200;
    void *frame[max_frames];
    int nptrs = ::backtrace(frame, max_frames);
    char **strings = ::backtrace_symbols(frame, nptrs);

    if(strings) {
        size_t len = 256;
        char *demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
        for(int i = 1; i < nptrs; ++i) {
            if(demangle) {
                char *left_par = nullptr;
                char *plus = nullptr;
                for(char *p = strings[i]; *p; ++p) {
                    ////////////
                }
            }
        }
    }
}

}

}