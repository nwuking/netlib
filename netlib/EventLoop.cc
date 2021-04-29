#include "./EventLoop.h"
#include "./Epoller.h"
#include "./Chnnel.h"
#include "./SockFunc.h"

#include <assert.h>
#include <sys/eventfd.h>

using namespace netlib;

namespace
{

__thread EventLoop *t_loopInThisThread = 0;             /// 记录当前线程的EventLoop;

const int cEpollTimeOutMs = 100000;

int createEventFd() {
  int fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if(fd < 0) {
    /// 分配一个描述符失败
    ::abort();
  }
  return fd;
}

}

EventLoop::EventLoop() 
    : _looping(false),
      _epoller(new Epoller(this)),
      _activeChnnels(cInitActiveChnnels),
      _currentActiveChnnel(nullptr),
      _epollReturnTime(),
      _tid(CurrentThread::tid()),
      _wakeFd(createEventFd()),
      _wakeChnnel(new Chnnel(this, _wakeFd)),
      _mutex(),
      _callPendingFunctors(false)
{
  /// _wakeFd->用于唤醒当前的loop
  if(t_loopInThisThread) {
    /// 当前的thread已经有一个EventLoop
    ;
  }
  else {
    t_loopInThisThread = this;
  }

  /// 注册_wakeChnnel
  _wakeChnnel->setReadCallBack(std::bind(&EventLoop::handleRead, this));
  _wakeChnnel->enableReading();
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

void EventLoop::queueInLoop(Functor cb) {
  /// 将回调函数cb放进_pendingFunctors中
  {
    MutexLock lock(_mutex);
    _pendingFunctors.push_back(std::move(cb));
  }

  if(!isInLoopThread() || _callPendingFunctors) {
    wakeUp();
  }
}

void EventLoop::wakeUp() {
  /// 通过_wakeFd唤醒当前EventLoop所在的线程
  /// 使其从epoll_wait()返回

  uint64_t one = 1;
  ssize_t n = netlib::write(_wakeFd, &one, sizeof one);
  if(n != sizeof one) {
    /// 
    ;
  }
}

void EventLoop::handleRead() {
  /// _wakeChnnel的回调函数
  uint64_t one = 1;
  ssize_t n = netlib::read(_wakeFd, &one, sizeof one);
  if(n != sizeof one) {
    /// read error
    ;
  }
}