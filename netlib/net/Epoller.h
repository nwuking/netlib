#ifndef EPOLLER_H_
#define EPOLLER_H_

#include "netlib/net/EventLoop.h"
#include "netlib/base/Time.h"

#include <sys/epoll.h>
#include <map>

namespace netlib
{

class Chnnel;

class Epoller : NonCopyAble
{
public:
    typedef std::vector<Chnnel*> ChnnelVec;

    Epoller(EventLoop *loop);
    ~Epoller();

    Time poll(ChnnelVec *activeChnnels, int timeoutMs);

    void updateChnnel(Chnnel *chnel);

    void update(int op, Chnnel *chnnel);

    void removeChnnel(Chnnel *chnnel);

    bool hasChnnel(Chnnel *chnnel) const;

    void assertInLoopThread() const {
        _ownLoop->assertInLoopThread();
    }

private:
    typedef std::vector<struct epoll_event> EpollEventVec;
    typedef std::map<int, Chnnel*> ChnnelMap;

    static const int cEventsInitSize = 8;
    
    void fillActiveChnnels(ChnnelVec *activeChnnels, int eventsNum) const;
    static const char* operationToString(int op);

    int _epollFd;
    EventLoop *_ownLoop;
    EpollEventVec _events;                                        /// 用于保存epoll_wait()返回的事件

    ChnnelMap _chnnels;
};

}


#endif