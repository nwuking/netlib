
#include "netlib/net/EventLoopThread.h"
#include "netlib/net/EventLoop.h"
#include "netlib/base/Thread.h"

#include <iostream>
#include <unistd.h>

int cnt = 0;
netlib::EventLoop *g_loop;

void printId() {
    std::cout << "pid=" << getpid() << " tid=" << netlib::CurrentThread::tid() << "\n"
              << "nowtime=" << netlib::Time::now().toString().c_str() << "\n"; 
}

void print(const char *msg) {
    std::cout << "msg " << netlib::Time::now().toString().c_str() << " "
              << msg << "\n";
    if(++cnt == 20) {
        g_loop->quit();
    }
}

void cancle(netlib::TimerId id) {
    g_loop->cancle(id);
    std::cout << "cancle at " << netlib::Time::now().toString().c_str() << "\n";
}

int main()
{
    printId();
    sleep(1);
    {
        netlib::EventLoop loop;
        g_loop = &loop;

        print("main");
        loop.runAfter(1, std::bind(print, "once1"));
        loop.runAfter(1.5, std::bind(print, "once1.5"));
        loop.runAfter(2.5, std::bind(print, "once2.5"));
        loop.runAfter(3.5, std::bind(print, "once3.5"));
        netlib::TimerId t45 = loop.runAfter(4.5, std::bind(print, "once4.5"));
        loop.runAfter(4.2, std::bind(cancle, t45));
        loop.runAfter(4.8, std::bind(cancle, t45));
        loop.runEvery(2, std::bind(print, "every2"));
        netlib::TimerId t3 = loop.runEvery(3, std::bind(print, "every3"));
        loop.runAfter(9.001, std::bind(cancle, t3));

        loop.loop();
        print("main loop exit!");
    }
    {
        netlib::EventLoopThread loopThread;
        netlib::EventLoop *loop = loopThread.startLoop();
        loop->runAfter(2, printId);
        sleep(3);
        print("thread loop exit");
    }
    return 0;
}