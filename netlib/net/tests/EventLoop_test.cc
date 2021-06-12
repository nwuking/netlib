#include "netlib/net/EventLoop.h"
#include "netlib/base/Thread.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

void callback() {
    std::cout << "callback():pid=" << getpid() << " tid=" << netlib::CurrentThread::tid() << "\n";
    netlib::EventLoop otherloop;
}

void threadFunc() {
    std::cout << "thread(): pid=" << ::getpid() << " tid=" << netlib::CurrentThread::tid() << "\n";
    assert(netlib::EventLoop::getEventLoopOfCurrentThead() == NULL);
    netlib::EventLoop loop;
    assert(netlib::EventLoop::getEventLoopOfCurrentThead() == &loop);
    loop.runAfter(1.0, callback);
    loop.loop();
}

int main()
{
    std::cout << "main(): pid=" << ::getpid() << " tid=" << netlib::CurrentThread::tid() << "\n";
    assert(netlib::EventLoop::getEventLoopOfCurrentThead() == NULL);

    netlib::EventLoop loop;
    assert(netlib::EventLoop::getEventLoopOfCurrentThead() == &loop);

    netlib::Thread thread(threadFunc);
    thread.start();

    loop.loop();
    return 0;
}