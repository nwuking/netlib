#include "./AsyncLogging.h"

#include <assert.h>

using namespace netlib;

AsyncLogging::AsyncLogging(const std::string &basename,
                            off_t rollSize,
                            int flushInterval)
    : _flushInterval(flushInterval),
      _basename(basename),
      _rollSize(rollSize),
      _running(false),
      _thread(std::bind(&AsyncLogging::threadFunc, this)),
      _mutex(),
      _cond(_mutex),
      _latch(1),
      _currentBuffer(new Buffer),
      _nextBuffer(new Buffer),
      _buffers()
{
    _currentBuffer->bzero();
    _nextBuffer->bzero();
    _buffers.reserve(16);
}

AsyncLogging::~AsyncLogging() {
  if(_running) {
    stop();
  }
}

void AsyncLogging::start() {
  _running = true;
  _thread.start();
  _latch.wait();
}

void AsyncLogging::stop() {
  _running = false;
}

void AsyncLogging::append(const char *msg, int len) {
  netlib::MutexLock lock(_mutex);

  if(_currentBuffer->vaild() > len) {
    _currentBuffer->append(msg, len);
  }
  else {
    _buffers.push_back(std::move(_currentBuffer));

    if(_nextBuffer) {
      _currentBuffer = std::move(_nextBuffer);
    }
    else {
      _currentBuffer.reset(new Buffer);
    }

    _currentBuffer->append(msg, len);
    _cond.notify();
  }
}

void AsyncLogging::threadFunc() {
  assert(_running == true);
  _latch.countDown();

  //
}
