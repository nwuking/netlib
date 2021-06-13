#include "netlib/net/EventLoopThread.h"
#include "netlib/net/EventLoop.h"
#include "netlib/base/Thread.h"
#include "netlib/base/CountDownLatch.h"

#include <iostream>
#include <unistd.h>

void print(netlib::EventLoop *loop = NULL) {
    std::cout << "print(): pid=" << getpid()
              << " tid=" << netlib::CurrentThread::tid()
              << " EventLoop=" << loop << "\n"; 
}

void quit(netlib::EventLoop *loop) {
    print(loop);
    loop->quit();
}

int main() 
{
    print();
    {
        netlib::EventLoopThread thr1;
    }

    {
        netlib::EventLoopThread thr2;
        netlib::EventLoop *loop = thr2.startLoop();
        loop->runInLoop(std::bind(print, loop));
        sleep(2);
    }

    {
        netlib::EventLoopThread thr3;
        netlib::EventLoop *loop = thr3.startLoop();
        loop->runInLoop(std::bind(quit, loop));
        sleep(2);
    }
    return 0;
}