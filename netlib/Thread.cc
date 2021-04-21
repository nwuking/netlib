#include "./Thread.h"

#include <assert.h>

namespace netlib
{

struct ThreadData
{
    /// 
    typedef netlib::Thread::ThreadFunc ThreadFunc;
    ThreadFunc _func;
    pid_t *_tid;

    ThreadData(ThreadFunc func, pid_t *tid)
        : _func(std::move(func)),
          _tid(tid)
    {
    }

    void runInThread();
};

void* startRoutine(void *arg) {
    /// 一个线程的起始函数
    /// pthread_create()的第三参数
}

Thread::Thread(ThreadFunc tFunc)
    : _start(false),
      _join(false),
      _pthreadId(0),
      _tid(0),
      _threadFunc(std::move(tFunc))
{
}

void Thread::start() {
    /// 创建一个实际线程
    assert(!_start);
    _start = true;

    if(::pthread_create(&_pthreadId, nullptr, &startRoutine, nullptr)) {
        /// return_val 不为0，调用失败
    }
}


}