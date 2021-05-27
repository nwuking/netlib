#ifndef ASYNCLOGGING_H_
#define ASYNCLOGGING_H_

#include "./LogStream.h"
#include "./Thread.h"
#include "./Mutex.h"
#include "./Condition.h"
#include "./CountDownLatch.h"

#include <vector>
#include <memory>
#include <atomic>

namespace netlib
{

class AsyncLogging : public NonCopyAble
{
public:
    AsyncLogging(const std::string &basename, off_t rollSize, int flushInterval = 3);
    ~AsyncLogging();

    void start();
    void stop();

    void append(const char *msg, int len);

private:

    void threadFunc();

    typedef netlib::FixedBuffer<netlib::cLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int _flushInterval;                                       /// 冲刷日志的时间间隔
    const std::string _basename;    
    const off_t _rollSize;

    std::atomic<bool> _running;

    netlib::Thread _thread;
    netlib::Mutex _mutex;
    netlib::Condition _cond;
    netlib::CountDownLatch _latch;

    BufferPtr _currentBuffer;
    BufferPtr _nextBuffer;
    BufferVector _buffers;

};


}


#endif