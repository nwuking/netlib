#include "netlib/net/EventLoopThreadPool.h"
#include "netlib/net/EventLoop.h"
#include "netlib/net/EventLoopThread.h"

#include <assert.h>
#include <stdio.h>

using namespace netlib;


EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, const std::string &name)
    : _baseLoop(baseLoop),
      _name(name),
      _numThreads(0),
      _next(0),
      _started(false)
{
    /// _baseLoop-> 即主线程所在的那个EventLoop，当_numThreads==0时，
    /// threadPool可以传回一个_baseLoop，即主线程处理一切事务
}

EventLoopThreadPool::~EventLoopThreadPool() {
}

void EventLoopThreadPool::start(const ThreadInitCallBack &cb) {
    /// 初始化线程池、启用线程池
    /// param：cb->用于线程的初始化函数
    assert(!_started);
    _baseLoop->assertInLoopThread();

    _started = true;
    for(int i = 0; i < _numThreads; ++i) {
        /// 初始化线程池

        char buf[_name.size() + 32];
        snprintf(buf, sizeof(buf), "%s%d", _name.c_str(), i);
        //std::unique_ptr<EventLoopThread> t(new EventLoopThread(cb));
        EventLoopThread *t = new EventLoopThread(cb, buf);
        _threadPool.push_back(std::unique_ptr<EventLoopThread>(t));
        _loops.push_back(t->startLoop());
    }
    if(_numThreads == 0 && cb) {
        /// 线程池没有线程
        /// 主线程处理
        cb(_baseLoop);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    /// 获取一个线程所拥有的loop
    _baseLoop->assertInLoopThread();
    assert(_started);

    EventLoop *loop = _baseLoop;                    /// 线程池没有线程可用，返回主线程的loop
    if(!_loops.empty()) {
        loop = _loops[_next];
        ++_next;
        if(_next >= static_cast<int>(_loops.size())) {
            _next = 0;
        }
    }
    return loop;
}

EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashCode) {
    _baseLoop->assertInLoopThread();
    assert(_started);

    EventLoop *loop = _baseLoop;
    if(!_loops.empty()) {
        loop = _loops[hashCode%_loops.size()];
    }
    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops() {
    _baseLoop->assertInLoopThread();
    assert(_started);

    if(_loops.empty()) {
        return std::vector<EventLoop*>(1, _baseLoop);
    }
    else {
        return _loops;
    }
}