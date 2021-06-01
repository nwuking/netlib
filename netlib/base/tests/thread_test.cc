#include "netlib/base/Thread.h"
#include "netlib/base/CurrentThread.h"

#include <stdio.h>
#include <unistd.h>
#include <string>

void mysleep(int x) {
    timespec t = {x,0};
    nanosleep(&t, NULL);
}

void threadFunc1() {
    printf("threadFunc1--tid=%d\n", CurrentThread::tid());
}

void threadFunc2(int x) {
    printf("threadFunc2--tid=%d, x=%d\n", CurrentThread::tid(), x);
}

void threadFunc3() {
    printf("threadFunc3--tid=%d\n", CurrentThread::tid());
    mysleep(1);
}

class Foo
{
public:
    explicit Foo(double x)
                : _x(x) 
    {}

    void memberFunc1() {
        printf("memberFunc1-tid=%d, Foo::_x=%f\n", CurrentThread::tid(), _x);
    } 

    void memberFunc2(const std::string &test) {
        printf("memberFunc2-tid=%d, Foo::_x=%f, test=%s\n", 
                CurrentThread::tid(), _x, test.c_str());
    }
private:
    double _x;
};

using namespace netlib;

int main() {
    printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());

    netlib::Thread t1(threadFunc1);
    t1.start();
    printf("t1.tid=%d\n", t1.gettid());
    t1.join();

    netlib::Thread t2(std::bind(threadFunc2, 42),
                        "thread for free function with argument");
    t2.start();
    printf("t2.tid=%d\n", t2.gettid());
    t2.join();

    Foo foo(21.12);
    netlib::Thread t3(std::bind(&Foo::memberFunc1, &foo));
    t3.start();
    printf("t3.tid=%d\n", t3.gettid());
    t3.join();

    netlib::Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo), std::string("nwuking")));
    t4.start();
    printf("t4.tid=%d\n", t4.gettid());
    t4.join();

    {
        netlib::Thread t5(threadFunc3);
        t5.start();
    }

    mysleep(2);
    {
        netlib::Thread t6(threadFunc3);
        t6.start();
        mysleep(2);
    }
    sleep(2);
    printf("number of created threads %d\n", netlib::Thread::numCreated());
    return 0;
}