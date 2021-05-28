#ifndef THREAD_H_
#define THREAD_H_

/*
 *  一个线程
*/

#include "./CountDownLatch.h"
#include "./noncopyable.h"

#include <functional>
#include <string>
#include <pthread.h>

namespace netlib
{

class Thread : public NonCopyAble
{
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(ThreadFunc tFunc, const std::string &name = std::string());
    ~Thread();

    void start();
    int join();

    pid_t gettid() const {
        return _tid;
    }

    bool started() const {
        return _start;
    }

    const std::string& name() const {
        return _name;
    }

private:
    bool _start;
    bool _join;
    pthread_t _pthreadId;
    pid_t _tid;
    ThreadFunc _threadFunc;
    CountDownLatch _latch;
    std::string _name;
};

}


#endif