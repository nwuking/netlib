#ifndef EVENTLOOPTHREADPOOL_H_
#define EVENTLOOPTHREADPOOL_H_

/*
 * 线程池，一个线程一个EventLoop
 */

#include <vector>
#include <memory>
#include <functional>

namespace netlib
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallBack;

    EventLoopThreadPool(EventLoop *baseLoop);
    ~EventLoopThreadPool();

    void setNumThreads(int nums = 1) {
        /// 设置线程池的线程数
        _numThreads = nums;
    }

    void start(const ThreadInitCallBack &cb = ThreadInitCallBack());

    EventLoop* getNextLoop();

private:
    EventLoop *_baseLoop;                   
    int _numThreads;
    int _next;
    bool _started;

    std::vector<std::unique_ptr<EventLoopThread>> _threadPool;  /// 管理每个线程
    std::vector<EventLoop*> _loops;                             /// 管理每个线程的loop
};

}


#endif