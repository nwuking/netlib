#include "./Epoller.h"
#include "./Chnnel.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>

using namespace netlib;

namespace 
{
    /// Chnnel在Epoller中的状态标志
    const int cNew = -1;
    const int cAdded = 1;
    const int cDeleted = 2;
}


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

void Epoller::updateChnnel(Chnnel *chnnel) {
    _ownLoop->assertInLoopThread();
    const int flag = chnnel->flag();
    int fd = chnnel->getFd();

    if(flag == cNew || flag == cDeleted) {
        /// cNew表示chnnel is a new one
        /// cDeleted表示chnnel已经在当前的epoller中，但没被使用
        if(flag == cNew) {
            assert(_chnnels.find(fd) == _chnnels.end());
            _chnnels[fd] = chnnel;
        }
        else {
            assert(_chnnels.find(fd) != _chnnels.end());
            assert(_chnnels[fd] == chnnel);
        }

        /// add with EPOLL_CTL_ADD
        chnnel->setFlag(cAdded);
        update(EPOLL_CTL_ADD, chnnel);
    }
    else {
        /// chnnel已经存在在Epoller中，且被使用
        assert(_chnnels.find(fd) != _chnnels.end());
        assert(_chnnels[fd] == chnnel);
        assert(flag == cAdded);

        if(chnnel->isNonEvent()) {
            /// chnnel没有要注册的事件
            /// move chnnel之前注册的事件
            update(EPOLL_CTL_DEL, chnnel);
            chnnel->setFlag(cDeleted);
        }
        else {
            /// 注册监听事件
            update(EPOLL_CTL_MOD, chnnel);
        }
    }
}

void Epoller::update(int op, Chnnel *chnnel) {
    struct epoll_event event;
    ::bzero(&event, sizeof event);

    event.events = chnnel->getEvents();
    event.data.ptr = chnnel;

    int fd = chnnel->getFd();
    if(::epoll_ctl(_epollFd, op, fd, &event) < 0) {
        /// error
        ;
    }
}