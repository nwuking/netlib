#ifndef THREAD_H_
#define THREAD_H_

/*
 *  一个线程
*/

#include "./CountDownLatch.h"

#include <functional>

#include <pthread.h>

namespace netlib
{

class Thread
{
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(ThreadFunc tFunc);
    ~Thread();

    void start();
    int join();

    pid_t gettid() const {
        return _tid;
    }

    bool started() const {
        return _start;
    }

private:
    bool _start;
    bool _join;
    pthread_t _pthreadId;
    pid_t _tid;
    ThreadFunc _threadFunc;
    CountDownLatch _latch;
};

}


#endif