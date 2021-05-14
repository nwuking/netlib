#include "./Thread.h"
#include "./CurrentThread.h"

#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace netlib
{

pid_t gettid() {
    /// 获取线程的线程号
    /// glibc没有封装相应的系统调用
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void CurrentThread::cacheTid() {
    /// 在文件CurrentThread.h中声明
    if(t_cachedTid == 0) {
        t_cachedTid = netlib::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d", t_cachedTid);
    }
}

struct ThreadData
{
    /// 
    typedef netlib::Thread::ThreadFunc ThreadFunc;
    ThreadFunc _func;
    pid_t *_tid;
    CountDownLatch *_latch;

    ThreadData(ThreadFunc func, pid_t *tid, CountDownLatch *latch)
        : _func(std::move(func)),
          _tid(tid),
          _latch(latch)
    {
    }

    void runInThread() {
        *_tid = CurrentThread::tid();
        _tid = nullptr;

        /// 通知主线程可以走了
        _latch->countDown();
        _latch = nullptr;

        _func();
    }
};

void* startRoutine(void *arg) {
    /// 一个线程的起始函数
    /// pthread_create()的第三参数
    ThreadData *data = static_cast<ThreadData*>(arg);
    data->runInThread();
    delete data;
    return nullptr;
}

Thread::Thread(ThreadFunc tFunc)
    : _start(false),
      _join(false),
      _pthreadId(0),
      _tid(0),
      _threadFunc(std::move(tFunc)),
      _latch(1)
{
}

void Thread::start() {
    /// 创建一个实际线程
    assert(!_start);
    _start = true;
    ThreadData *data = new ThreadData(_threadFunc, &_tid, &_latch);
    if(::pthread_create(&_pthreadId, nullptr, &startRoutine, data)) {
        /// return_val 不为0，调用失败
        _start = false;
        delete data;

        /////
    }
    else {
        _latch.wait();
        assert(_tid > 0);
    }
}

int Thread::join() {
    assert(_start);
    assert(!_join);
    _join = true;
    return ::pthread_join(_pthreadId, nullptr);
}

}