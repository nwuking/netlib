#include "./EventLoop.h"
#include "./Epoller.h"
#include "./Chnnel.h"

#include <assert.h>

using namespace netlib;

const int cEpollTimeOutMs = 100000;

EventLoop::EventLoop() 
    : _looping(false),
      _epoller(new Epoller(this)),
      _activeChnnels(cInitActiveChnnels),
      _currentActiveChnnel(nullptr),
      _epollReturnTime(),
      _tid(CurrentThread::tid())
{
}

EventLoop::~EventLoop() {
}

void EventLoop::loop() {
  _looping = true;
  while(_looping) {
    _activeChnnels.clear();
    _epollReturnTime = _epoller->poll(&_activeChnnels, cEpollTimeOutMs);

    for(Chnnel *chnnel : _activeChnnels) {
      _currentActiveChnnel = chnnel;

      /// 分发事件
      _currentActiveChnnel->handleEvent();
    }
    _currentActiveChnnel = nullptr;
  }
}

void EventLoop::abortNotInLoopThread() {
  ////
  ;
}

void EventLoop::updateChnnel(Chnnel *chnnel) {
  assert(chnnel->owerLoop() == this);
  assertInLoopThread();
  _epoller->updateChnnel(chnnel);
}