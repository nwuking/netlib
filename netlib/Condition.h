#ifndef CONDITION_H_
#define CONDITION_H_

/*
 *  封装条件变量Cond
 */

#include "./Mutex.h"

#include <pthread.h>

namespace netlib
{

class Condition
{
public:
    Condition(Mutex &mutex)
        : _mutex(mutex)
    {
        ::pthread_cond_init(&_cond, nullptr);
    }
    ~Condition() {
        ::pthread_cond_destroy(&_cond);
    }

    void wait() {
        Mutex::ungetHolderMutex ug(_mutex);
        ::pthread_cond_wait(&_cond, _mutex.getMutex());
    }

    void waitForSeconds(int seconds) {
        Mutex::ungetHolderMutex ug(_mutex);
        struct timespec t;
        ::clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec += static_cast<time_t>(seconds);
        ::pthread_cond_timedwait(&_cond, _mutex.getMutex(), &t);
    }

    void notify() {
        ::pthread_cond_signal(&_cond);
    }

    void notifyAll() {
        ::pthread_cond_broadcast(&_cond);
    }

private:
    Mutex &_mutex;
    pthread_cond_t _cond;
};

}


#endif