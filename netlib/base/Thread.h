#ifndef THREAD_H_
#define THREAD_H_

/*
 *  一个线程
*/

#include "netlib/base/CountDownLatch.h"
#include "netlib/base/noncopyable.h"

#include <functional>
#include <string>
#include <pthread.h>
#include <atomic>

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

    static int numCreated() {
        return _numCreated;
    }

private:
    void setDefaultName();

    bool _start;
    bool _join;
    pthread_t _pthreadId;
    pid_t _tid;
    ThreadFunc _threadFunc;
    CountDownLatch _latch;
    std::string _name;

    static std::atomic<int> _numCreated;
};

}


#endif