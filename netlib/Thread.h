#ifndef THREAD_H_
#define THREAD_H_

/*
 *  一个线程
*/

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

private:
    bool _start;
    bool _join;
    pthread_t _pthreadId;
    pid_t _tid;
    ThreadFunc _threadFunc;
};

}


#endif