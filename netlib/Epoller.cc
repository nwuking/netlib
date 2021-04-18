#include "./Epoller.h"
#include "./Chnnel.h"

#include <unistd.h>
#include <assert.h>

using namespace netlib;

typedef Epoller::ChnnelVec ChnnelVec;

Epoller::Epoller(EventLoop *loop)
    : _epollFd(::epoll_create1(EPOLL_CLOEXEC)),
      _ownLoop(loop),
      _events(cEventsInitSize)
{
}

Epoller::~Epoller() {
    ::close(_epollFd);
}

/// typedef union epoll_data {
///     void *ptr;
///     int fd;
///     uint32_t u32;
///     uint64_t u64;
/// }epoll_data_t;
/// 
/// struct epoll_event {
///     uint32_t events;            /// Epoll events
///     epoll_data_t data;          /// User datavariable
/// };

Time Epoller::poll(ChnnelVec *activeChnnels, int timeoutMs) {
    int eventsNum = ::epoll_wait(_epollFd, &*_events.begin(), static_cast<int>(_events.size()), timeoutMs);
    int saveErrno = errno;
    Time now(Time::now());
    if(eventsNum > 0) {
        /// 有响应事件，将响应事件push activeChnnels
        fillActiveChnnels(activeChnnels, eventsNum);
        if(eventsNum == static_cast<int>(_events.size())) {
            /// 预先处理
            _events.resize(_events.size()*2);
        }
    }
    else if(eventsNum == 0) {
        /// nothing
    }
    else {
        /// epoll_wait error
        if(errno != EINTR) {
            /// 不是发生了中断，恢复原来erron的值
            errno = saveErrno;
        }
    }
    return now;
}

void Epoller::fillActiveChnnels(ChnnelVec *activeChnnels, int eventsNum) {
    assert(_events.size() >= static_cast<size_t>(eventsNum));
    for(int i = 0; i < eventsNum; ++i) {
        Chnnel *chnnel = static_cast<Chnnel*>(_events[i].data.ptr);
        chnnel->setRevents(_events[i].events);
        activeChnnels->push_back(chnnel);
    }
}