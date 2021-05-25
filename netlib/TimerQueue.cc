#include "./TimerQueue.h"
#include "./EventLoop.h"
#include "./TimerId.h"
#include "./Logging.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <assert.h>
#include <algorithm>
#include <string.h>

namespace netlib
{

struct timespec howMuchTimeFromNow(Time when) {
    int64_t microSeconds = when.getMicroSecondsSinceEpoch() - 
                    Time::now().getMicroSecondsSinceEpoch();
    
    if(microSeconds < 100) {
        microSeconds = 100;
    }
    struct timespec ts;
    ts.tv_nsec = static_cast<time_t>(microSeconds / Time::cMicroSecondsPerSecond);
    ts.tv_sec = static_cast<long>(microSeconds%Time::cMicroSecondsPerSecond *1000);
    return ts;
}

int createTimerFd() {
    int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    if(fd < 0) {
        LOG_SYSFATAL << "Failed in timerfd_create()";
    }

    return fd;
}

void readTimerFd(int fd, Time now) {
    uint64_t pp;
    ssize_t n = ::read(fd, &pp, sizeof pp);

    LOG_TRACE << "TimerQueue::handleRead() -" << pp << " at " << now.toFormattedString();

    if(n != sizeof(pp)) {
        LOG_ERROR << "TimerQueue::handleRead() " << n << "bytes instead of 8 bytes";
    }
}

/// struct timespec {
///     time_t tv_sec;
///     long tv_nsec;
/// };

/// struct itimerspec {
///     struct timespec it_interval;
///     struct timespec it_value;
/// };

void resetTimerFd(int fd, Time expiration) {
    struct itimerspec newValue;
    struct itimerspec oldValue;
    ::bzero(&newValue, sizeof newValue);
    ::bzero(&oldValue, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(fd, 0, &newValue, &oldValue);
    if(ret) {
        LOG_SYSERR << "timerfd_settime()";
    }
}

}

using namespace netlib;

TimerQueue::TimerQueue(EventLoop *loop)
    : _loop(loop),
      _timerFd(netlib::createTimerFd()),
      _timerFdChnnel(loop, _timerFd),
      _timers(),
      _callingExpiredTimer(false)
{
    _timerFdChnnel.setReadCallBack(std::bind(&TimerQueue::handleRead, this));
    _timerFdChnnel.enableReading();
}

TimerQueue::~TimerQueue() {
    _timerFdChnnel.diableAll();
    _timerFdChnnel.remove();

    ::close(_timerFd);
    for(const Entry &it : _timers) {
        delete it.second;
    }
}

TimerId TimerQueue::addTimer(TimerCallBack cb, Time when, double interval) {
    Timer *timer = new Timer(std::move(cb), when, interval);
    _loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
    return TimerId(timer, timer->sequence());
}

void TimerQueue::addTimerInLoop(Timer *timer) {
    _loop->assertInLoopThread();
    bool earliestChange = insert(timer);
    if(earliestChange) {
        resetTimerFd(_timerFd, timer->expiration());
    }
}

void TimerQueue::cancleTimer(TimerId timerId) {
    _loop->runInLoop(std::bind(&TimerQueue::cancleInLoop, this, timerId));
}

void TimerQueue::cancleInLoop(TimerId timerId) {
    _loop->assertInLoopThread();
    assert(_timers.size() == _activeTimers.size());
    ActiveTimer timer(timerId._timer, timerId._sequence);
    ActiveTimerSet::iterator it = _activeTimers.find(timer);

    if(it != _activeTimers.end()) {
        size_t n = _timers.erase(Entry(it->first->expiration(), it->first));
        assert(n == 1);
        (void)n;
        delete it->first;
        _activeTimers.erase(it);
    }
    else if(_callingExpiredTimer) {
        _canllingTimers.insert(timer);
    }
    
    assert(_timers.size() == _activeTimers.size());
}

void TimerQueue::handleRead() {
    _loop->assertInLoopThread();
    Time now(Time::now());
    readTimerFd(_timerFd, now);

    std::vector<Entry> expired = getExpired(now);
    _callingExpiredTimer = true;
    _canllingTimers.clear();
    for(const Entry &it : expired) {
        it.second->run();
    }
    _callingExpiredTimer = false;
    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Time &now) {
    assert(_timers.size() == _activeTimers.size());

    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator end = _timers.lower_bound(sentry);
    assert(end == _timers.end() || now < end->first);
    std::copy(_timers.begin(), end, std::back_inserter(expired));
    _timers.erase(_timers.begin(), end);

    for(const Entry &it : expired) {
        ActiveTimer timer(it.second, it.second->sequence());
        size_t n = _activeTimers.erase(timer);
        assert(n == 1);
        (void)n;
    }

    assert(_timers.size() == _activeTimers.size());
    return expired;
}

void TimerQueue::reset(const std::vector<Entry> &expired, Time &now) {
    Time nextExpired;
    for(const Entry &it : expired) {
        ActiveTimer timer(it.second, it.second->sequence());

        if(it.second->repeat() && _canllingTimers.find(timer) == _canllingTimers.end()) {
            it.second->restart(now);
            insert(it.second);
        }
        else {
            delete it.second;
        }
    }

    if(!_timers.empty()) {
        nextExpired = _timers.begin()->second->expiration();
    }

    if(nextExpired.vaild()) {
        resetTimerFd(_timerFd, nextExpired);
    }
}

bool TimerQueue::insert(Timer *timer) {
    _loop->assertInLoopThread();
    assert(_timers.size() == _activeTimers.size());

    Time when = timer->expiration();
    bool earliestChange = false;
    TimerList::iterator it = _timers.begin();
    if(it == _timers.end() || when < it->first) {
        earliestChange = true;
    }
    _timers.insert(Entry(when, timer));
    _activeTimers.insert(ActiveTimer(timer, timer->sequence()));
    return earliestChange;
}