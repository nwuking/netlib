#include "netlib/base/Mutex.h"
#include "netlib/base/Thread.h"
#include "netlib/base/CountDownLatch.h"
#include "netlib/base/Time.h"

#include <vector>
#include <iostream>
#include <memory>

netlib::Mutex g_mutex;
std::vector<int> g_vec;
const int kCount = 10*1000*1000;

void threadFunc() {
    for(int i = 0; i < kCount; ++i) {
        netlib::MutexLock lock(g_mutex);
        g_vec.push_back(i);
    }
}

int foo() __attribute__ ((noinline));

int g_count = 0;
int foo() {
    netlib::MutexLock lock(g_mutex);
    if(!g_mutex.isLockedByThisThread()) {
        std::cout << "FAIL\n";
        return -1;
    }
    ++g_count;
    return 0;
}

int main() {
    const int kMaxThreads = 15;
    g_vec.reserve(kMaxThreads * kCount);

    netlib::Time start(netlib::Time::now());
    for(int i = 0; i < kCount; ++i) {
        g_vec.push_back(i);
    }

    std::cout << "single thread without lock " << netlib::timeDifference(netlib::Time::now(),
                                                                        start) 
                                               << "\n";
    
    start = netlib::Time::now();
    threadFunc();
    std::cout << "single thread with lock " << netlib::timeDifference(netlib::Time::now(),
                                                                        start) 
                                               << "\n";

    /// 多线程
    for(int j = 1; j < kMaxThreads; ++j) {
        std::vector<std::unique_ptr<netlib::Thread>> threads;
        g_vec.clear();
        start = netlib::Time::now();
        for(int i = 0; i < j; ++i) {
            threads.emplace_back(new netlib::Thread(&threadFunc));
            threads.back()->start();
        }
        for(int i = 0; i < j; ++i) {
            threads[i]->join();
        }
        std::cout << j << " threads with lock " 
                  << netlib::timeDifference(netlib::Time::now(), start) << "\n";
    }
    return 0;
}