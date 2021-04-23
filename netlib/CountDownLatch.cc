#include "./CountDownLatch.h"


using namespace netlib;


CountDownLatch::CountDownLatch(int count)
    : _mutex(),
      _cond(_mutex),
      _count(count)
{
}

void CountDownLatch::wait() {
    MutexLock lock(_mutex);

    while(_count > 0) {
        _cond.wait();
    }
}

void CountDownLatch::countDown() {
    MutexLock lock(_mutex);
    --_count;
    if(_count == 0) {
        _cond.notifyAll();
    }
}