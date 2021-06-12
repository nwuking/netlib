#include "netlib/net/EventLoop.h"
#include "netlib/net/Epoller.h"
#include "netlib/net/Chnnel.h"
#include "netlib/net/SockFunc.h"
#include "netlib/base/Logging.h"
#include "netlib/net/TimerQueue.h"

#include <assert.h>
#include <sys/eventfd.h>
#include <algorithm>

using namespace netlib;

namespace
{

__thread EventLoop *t_loopInThisThread = 0;             /// 记录当前线程的EventLoop;

const int cEpollTimeOutMs = 1000;

int createEventFd() {
  int fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if(fd < 0) {
    /// 分配一个描述符失败
    LOG_SYSERR << "Failed in eventfd!";
    ::abort();
  }
  return fd;
}

}

EventLoop* EventLoop::getEventLoopOfCurrentThead() {
  return t_loopInThisThread;
}

EventLoop::EventLoop() 
    : _looping(false),
      _quit(false),
      _epoller(new Epoller(this)),
      _activeChnnels(cInitActiveChnnels),
      _currentActiveChnnel(nullptr),
      _epollReturnTime(),
      _tid(CurrentThread::tid()),
      _wakeFd(createEventFd()),
      _wakeChnnel(new Chnnel(this, _wakeFd)),
      _mutex(),
      _callPendingFunctors(false),
      _eventHandle(false),
      _timerQueue(new TimerQueue(this))
{
  LOG_DEBUG << "EventLoop created " << this << "in thread" << _tid;

  /// _wakeFd->用于唤醒当前的loop
  if(t_loopInThisThread) {
    /// 当前的thread已经有一个EventLoop
    LOG_FATAL << "Anothre EventLoop " << t_loopInThisThread
              << " exitsts in this thread " << _tid;
  }
  else {
    t_loopInThisThread = this;
  }

  /// 注册_wakeChnnel
  _wakeChnnel->setReadCallBack(std::bind(&EventLoop::handleRead, this));
  _wakeChnnel->enableReading();
}

EventLoop::~EventLoop() {
  LOG_DEBUG << "EventLoop " << this << " of thread " << _tid
            << " destructs in thread" << CurrentThread::tid();

  _wakeChnnel->diableAll();
  _wakeChnnel->remove();

  ::close(_wakeFd);
  t_loopInThisThread = NULL;
}

void EventLoop::loop() {
  assert(!_looping);
  assertInLoopThread();
  _looping = true;
  _quit = false;
  while(!_quit) {
    _activeChnnels.clear();
    _epollReturnTime = _epoller->poll(&_activeChnnels, cEpollTimeOutMs);

    if(Logger::logLevel() <= Logger::TRACE) {
      printActiveChnnels();
    }

    _eventHandle = true;
    for(Chnnel *chnnel : _activeChnnels) {
      _currentActiveChnnel = chnnel;

      /// 分发事件
      _currentActiveChnnel->handleEvent(_epollReturnTime);
    }
    
    _currentActiveChnnel = nullptr;
    _eventHandle = false;
    dpPendingFunctors();
  }

  LOG_TRACE << "EventLoop " << this << "stop looping";
  _looping = false;
}

void EventLoop::quit() {
  _quit = true;

  if(!isInLoopThread()) {
    wakeUp();
  }
}

void EventLoop::abortNotInLoopThread() {
  ////
  LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in _tid " << _tid
            << ", current thread id = " << CurrentThread::tid(); 
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
    LOG_ERROR << "EventLoop::wakeUp() writes " << n << " bytes insteal of 8";
  }
}

void EventLoop::runInLoop(Functor cb) {
  if(isInLoopThread()) {
    cb();
  }
  else {
    queueInLoop(std::move(cb));
  }
}

void EventLoop::removeChnnel(Chnnel *chnnel) {
  /// 从EventLoop中移除Chnnel
  assert(chnnel->owerLoop() == this);
  assertInLoopThread();

  if(_eventHandle) {
    /// 有事件正在派发
    assert(_currentActiveChnnel == chnnel || 
        std::find(_activeChnnels.begin(), _activeChnnels.end(), chnnel) == _activeChnnels.end());
  }

  _epoller->removeChnnel(chnnel);
}

bool EventLoop::hasChnnel(Chnnel *chnnel) {
  assert(chnnel->owerLoop() == this);
  abortNotInLoopThread();
  return _epoller->hasChnnel(chnnel);
}

TimerId EventLoop::runAt(Time when, TimerCallBack cb) {
  return _timerQueue->addTimer(std::move(cb), when, 0.0);
}

TimerId EventLoop::runAfter(double delay, TimerCallBack cb) {
  Time time(addTime(Time::now(), delay));
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, TimerCallBack cb) {
  Time time(addTime(Time::now(), interval));
  return _timerQueue->addTimer(std::move(cb), time, interval);
}

void EventLoop::cancle(TimerId timerId) {
  _timerQueue->cancleTimer(timerId);
}

void EventLoop::handleRead() {
  /// _wakeChnnel的回调函数
  uint64_t one = 1;
  ssize_t n = netlib::read(_wakeFd, &one, sizeof one);
  if(n != sizeof one) {
    /// read error
    LOG_ERROR << "EventLoop::handRead() reads " << n << " bytes instead of 8";
  }
}

void EventLoop::printActiveChnnels() const {
  for(const Chnnel *chnnel : _activeChnnels) {
    LOG_TRACE << "{" << chnnel->reventsToString() << "} ";
  }
}

void EventLoop::dpPendingFunctors() {
  std::vector<Functor> vec;
  _callPendingFunctors = true;

  {
    MutexLock lock(_mutex);
    vec.swap(_pendingFunctors);
  }

  for(const Functor &func : vec) {
    func();
  }
  _callPendingFunctors = false;
}