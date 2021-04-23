#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_

/*
 *  一个计数器，用于线程间的同步，封装了底层的mutex和cond
 */

#include "./Mutex.h"
#include "./Condition.h"

namespace netlib
{

class CountDownLatch
{
public:
    explicit CountDownLatch(int count = 1);
    ~CountDownLatch() = default;

    void wait();

    void countDown();

private:
    Mutex _mutex;
    Condition _cond;
    int _count;
};

}


#endif