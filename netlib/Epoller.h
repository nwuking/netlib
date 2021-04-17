#ifndef EPOLLER_H_
#define EPOLLER_H_

#include "./EventLoop.h"
#include "./Time.h"

#include <sys/epoll.h>

namespace netlib
{

class Chnnel;

class Epoller
{
public:
    typedef std::vector<Chnnel*> ChnnelVec;

    Epoller(EventLoop *loop);
    ~Epoller();

    Time poll(ChnnelVec *activeChnnels, int timeoutMs);

private:
    typedef std::vector<struct epoll_event> EpollEventVec;

    static const int cEventsInitSize = 8;
    
    void fillActiveChnnels(ChnnelVec *activeChnnels, int eventsNum);

    int _epollFd;
    EventLoop *_ownLoop;
    EpollEventVec _events;                                        /// 用于保存epoll_wait()返回的事件
};

}


#endif