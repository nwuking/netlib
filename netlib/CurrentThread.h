#ifndef CURRENTTHREAD_H_
#define CURRENTTHREAD_H_

/*
 *   
 */

namespace netlib
{

namespace CurrentThread
{

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;

void cacheTid();

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

}


}


#endif