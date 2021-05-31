#include "./Thread.h"
#include "./CurrentThread.h"
#include "./Exception.h"
#include "./Logging.h"

#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <stdio.h>

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
    std::string _name;

    ThreadData(ThreadFunc func, pid_t *tid, CountDownLatch *latch, const std::string &name)
        : _func(std::move(func)),
          _tid(tid),
          _latch(latch),
          _name(name)
    {
    }

    void runInThread() {
        *_tid = CurrentThread::tid();
        _tid = nullptr;

        /// 通知主线程可以走了
        _latch->countDown();
        _latch = nullptr;

        CurrentThread::t_threadName = _name.empty() ? "netlibThread" : _name.c_str();

        /// 给线程命名
        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);

        try {
            _func();
            CurrentThread::t_threadName = "finished";
        }
        catch(const Exception &ex) {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception catch in thread %s\n", _name.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            ::abort();
        }
        catch(const std::exception &ex) {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception catch in thread %s\n", _name.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            ::abort();
        }
        catch(...) {
            CurrentThread::t_threadName = "unkonwn crash";
            fprintf(stderr, "unknown exception catch in thread %s\n", _name.c_str());
            throw;
        }
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

Thread::Thread(ThreadFunc tFunc, const std::string &name)
    : _start(false),
      _join(false),
      _pthreadId(0),
      _tid(0),
      _threadFunc(std::move(tFunc)),
      _latch(1),
      _name(name)
{
    setDefaultName();
}

Thread::~Thread() {
    if(_start && !_join) {
        ::pthread_detach(_pthreadId);
    }
}

void Thread::setDefaultName() {
    int num = ++_numCreated;
    if(_name.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        _name = buf;
    }
}

void Thread::start() {
    /// 创建一个实际线程
    assert(!_start);
    _start = true;
    ThreadData *data = new ThreadData(_threadFunc, &_tid, &_latch, _name);
    if(::pthread_create(&_pthreadId, nullptr, &startRoutine, data)) {
        /// return_val 不为0，调用失败
        _start = false;
        delete data;

        LOG_SYSFATAL << "failed in pthread_create";
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