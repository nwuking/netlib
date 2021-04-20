#ifndef EVENTLOOPTHREADPOOL_H_
#define EVENTLOOPTHREADPOOL_H_

/*
 * 线程池，一个线程一个EventLoop
 */

#include <vector>
#include <memory>

namespace netlib
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
public:
    EventLoopThreadPool(EventLoop *baseLoop);
    ~EventLoopThreadPool();

private:
    EventLoop *_baseLoop;                   
    int _numThreads;
    int _next;

    std::vector<std::unique_ptr<EventLoopThread>> _threadPool;
};

}


#endif