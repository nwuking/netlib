#include "netlib/base/AsyncLogging.h"
#include "netlib/base/LogFile.h"
#include "netlib/base/Time.h"

#include <assert.h>
#include <stdio.h>

using namespace netlib;

AsyncLogging::AsyncLogging(const std::string &basename,
                            off_t rollSize,
                            int flushInterval)
    : _flushInterval(flushInterval),
      _basename(basename),
      _rollSize(rollSize),
      _running(false),
      _thread(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
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
  _cond.notify();
  _thread.join();
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

  LogFile output(_basename, _rollSize, _flushInterval);
  BufferPtr newBuffer1(new Buffer);
  BufferPtr newBuffer2(new Buffer);
  newBuffer1->bzero();
  newBuffer2->bzero();
  BufferVector buffersToWrite;
  buffersToWrite.reserve(16);

  while(_running) {
    assert(newBuffer1 && newBuffer1->length() == 0);
    assert(newBuffer2 && newBuffer2->length() == 0);
    assert(buffersToWrite.empty());

    {
      netlib::MutexLock lock(_mutex);
      if(_buffers.empty()) {
        _cond.waitForSeconds(_flushInterval);
      }

      _buffers.push_back(std::move(_currentBuffer));
      _currentBuffer = std::move(newBuffer1);
      buffersToWrite.swap(_buffers);
      if(!_nextBuffer) {
        _nextBuffer = std::move(newBuffer2);
      }
    }

    assert(!buffersToWrite.empty());

    if(buffersToWrite.size() > 25) {
      char buf[256];
      ::snprintf(buf, sizeof buf, "Drop log message at %s, %zd large buffers\n",
                Time::now().toFormattedString(), buffersToWrite.size()-2);
      
      buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
    }

    for(const auto &buffer : buffersToWrite) {
      output.append(buffer->data(), buffer->length());
    }

    if(buffersToWrite.size() > 2) {
      buffersToWrite.resize(2);
    }

    if(!newBuffer1) {
      assert(!buffersToWrite.empty());
      newBuffer1 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer1->reset();
    }

    if(!newBuffer2) {
      assert(!buffersToWrite.empty());
      newBuffer2 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer2->reset();
    }

    buffersToWrite.clear();
    output.flush();
  }  
  output.flush();
}
