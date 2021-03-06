#ifndef CURRENTTHREAD_H_
#define CURRENTTHREAD_H_

/*
 *   
 */

#include <string>

namespace netlib
{

namespace CurrentThread
{

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char *t_threadName;

void cacheTid();

std::string stackTrace(bool demangle);

inline int tid() {
    if(t_cachedTid == 0) {
        cacheTid();
    }
    return t_cachedTid;
}

inline const char* tidString() {
    return t_tidString;
}

inline int tidStringLength() {
    return t_tidStringLength;
}

inline const char* name() {
    return t_threadName;
}

}


}


#endif