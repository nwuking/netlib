#include "./EventLoopThreadPool.h"
#include "./EventLoop.h"


using namespace netlib;


EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop)
    : _baseLoop(baseLoop),
      _numThreads(0),
      _next(0)
{
    /// _baseLoop-> 即主线程所在的那个EventLoop，当_numThreads==0时，
    /// threadPool可以传回一个_baseLoop，即主线程处理一切事务
}