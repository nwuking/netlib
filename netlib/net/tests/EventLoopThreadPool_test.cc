#include "netlib/net/EventLoop.h"
#include "netlib/net/EventLoopThreadPool.h"
#include "netlib/base/Thread.h"

#include <iostream>
#include <assert.h>
#include <unistd.h>

void print(netlib::EventLoop *loop = NULL) {
    std::cout << "print: pid=" << getpid()
              << " tid=" << netlib::CurrentThread::tid()
              << " EventLoop=" << loop << "\n";
}

void init(netlib::EventLoop *loop) {
    std::cout << "print: pid=" << getpid()
              << " tid=" << netlib::CurrentThread::tid()
              << " EventLoop=" << loop << "\n";
}

int main() 
{
    print();
    netlib::EventLoop loop;
    loop.runAfter(11, std::bind(&netlib::EventLoop::quit, &loop));

    {
        std::cout << "Single thread = " << &loop << "\n";
        netlib::EventLoopThreadPool model(&loop, "Single");
        model.setNumThreads(0);
        model.start(init);
        assert(model.getNextLoop() == &loop);
        assert(model.getNextLoop() == &loop);
    }

    {
        std::cout << "Another thread:\n";
        netlib::EventLoopThreadPool model(&loop, "Another");
        model.setNumThreads(1);
        model.start(init);
        netlib::EventLoop *nextLoop = model.getNextLoop();
        nextLoop->runAfter(2, std::bind(print, nextLoop));
        assert(nextLoop != &loop);
        assert(nextLoop == model.getNextLoop());
        assert(nextLoop == model.getNextLoop());
    }

    {
        std::cout << "Three threads:\n";
        netlib::EventLoopThreadPool model(&loop, "Three");
        model.setNumThreads(3);
        model.start(init);
        netlib::EventLoop *nextLoop = model.getNextLoop();
        nextLoop->runInLoop(std::bind(print, nextLoop));
        assert(nextLoop != &loop);
        assert(nextLoop != model.getNextLoop());
        assert(nextLoop != model.getNextLoop());
        assert(nextLoop == model.getNextLoop());
    }
    return 0;
}