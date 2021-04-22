#ifndef MUTEX_H_
#define MUTEX_H_

/*
 *  封装线程同步原语mutex
 */

#include <pthread.h>

#include "./CurrentThread.h"

namespace netlib
{

class Mutex
{
public:
    Mutex() : _holder(0) {
        /// 初始化mutex
        ::pthread_mutex_init(&_mutex, nullptr);
    }
    ~Mutex() {
        ::pthread_mutex_destroy(&_mutex);
    }

    void lock() {
        ::pthread_mutex_lock(&_mutex);

        /// 记录哪个线程拥有了这把锁
        getHolder();
    }

    void unlock() {
        ungetHolder();
        ::pthread_mutex_unlock(&_mutex);
    }

    pthread_mutex_t* getMutex() {
        return &_mutex;
    }

private:

    friend class Condition;

    class ungetHolderMutex
    {

    public:
        ungetHolderMutex(Mutex &mutex) : _mutex(mutex) {
            _mutex.ungetHolder();
        }
        ~ungetHolderMutex() {
            _mutex.getHolder();
        }
    private:
        Mutex &_mutex;
    };

    void getHolder() {
        /// 记录哪个线程获得了锁
        _holder = CurrentThread::tid();
    }
    void ungetHolder() {
        _holder = 0;
    }

    pthread_mutex_t _mutex;
    pid_t _holder;                      /// 记录哪个线程拥有这个mutex
};


class MutexLock
{
public:
    MutexLock(Mutex &mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLock() {
        _mutex.unlock();
    }
private:
    Mutex &_mutex;
};


}


#endif