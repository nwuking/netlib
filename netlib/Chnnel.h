#ifndef CHNNEL_H_
#define CHNNEL_H_

#include "./EventLoop.h"

namespace netlib
{

class Chnnel
{
public:
    Chnnel(EventLoop *loop, int fd);
    ~Chnnel();

    void handleEvent();

    void set_revents(int revents) {
        _revevts = revents;
    }

private:
    EventLoop *_ownLoop;
    int _fd;
    int _events;
    int _revevts;
};

}


#endif