#ifndef MUTEX_H_
#define MUTEX_H_

/*
 *  封装线程同步原语mutex
 */

#include <pthread.h>

namespace netlib
{

class Mutex
{
public:
    Mutex();
    ~Mutex();

private:
    pthread_mutex_t _mutex;
    pid_t _holder;                      /// 记录哪个线程拥有这个mutex
};


}


#endif