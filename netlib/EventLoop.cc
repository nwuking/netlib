#include "./EventLoop.h"
#include "./Epoller.h"
#include "./Chnnel.h"


using namespace netlib;

const int cEpollTimeOutMs = 100000;

EventLoop::EventLoop() 
    : _looping(false),
      _epoller(new Epoller(this)),
      _activeChnnels(cInitActiveChnnels),
      _currentActiveChnnel(nullptr),
      _epollReturnTime()
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