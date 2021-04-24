#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

/*
 *  reactor 模型的循环部分
 */

#include "./Time.h"
#include "./CurrentThread.h"

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
    EventLoop();
    ~EventLoop();

    void loop();

    void setEpollTimeOut(int64_t second, int64_t microSeconds);

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

    typedef std::vector<Chnnel*> ChnnelVec;

    static const int cInitActiveChnnels = 8;

    std::atomic<bool> _looping;
    std::unique_ptr<Epoller> _epoller;                  /// 整个程序只有一个Epoller Object
    ChnnelVec _activeChnnels;                           /// 用于保存发生响应的事件(Chnnel)

    Chnnel *_currentActiveChnnel;
    Time _epollReturnTime;

    const pid_t _tid;
};

}

#endif