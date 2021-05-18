#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

/*
 *  reactor 模型的循环部分
 */

#include "./Time.h"
#include "./CurrentThread.h"
#include "./Mutex.h"

#include <vector>
#include <memory>
#include <atomic>

namespace netlib
{

class Chnnel;
class Epoller;

class EventLoop
{
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void loop();

    void quit();

    void setEpollTimeOut(int64_t second, int64_t microSeconds);

    void updateChnnel(Chnnel *chnnel);

    void queueInLoop(Functor cb);

    void wakeUp();

    void runInLoop(Functor cb);

    void removeChnnel(Chnnel *chnnel);

    bool hasChnnel(Chnnel *chnnel);

    void assertInLoopThread() {
        if(!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    void quit() {
        _looping = false;
    }

    bool isInLoopThread() const {
        return _tid == CurrentThread::tid();
    }

private:
    void abortNotInLoopThread();
    void handleRead();
    void printActiveChnnels() const;
    void dpPendingFunctors();

    typedef std::vector<Chnnel*> ChnnelVec;

    static const int cInitActiveChnnels = 8;

    bool _looping;
    std::atomic<bool> _quit;
    std::unique_ptr<Epoller> _epoller;                  /// 整个程序只有一个Epoller Object
    ChnnelVec _activeChnnels;                           /// 用于保存发生响应的事件(Chnnel)

    Chnnel *_currentActiveChnnel;
    Time _epollReturnTime;

    const pid_t _tid;

    int _wakeFd;
    std::unique_ptr<Chnnel> _wakeChnnel;

    Mutex _mutex;
    std::vector<Functor> _pendingFunctors;                  /// 保存未执行得到回调函数
    bool _callPendingFunctors;
    bool _eventHandle;
};

}

#endif