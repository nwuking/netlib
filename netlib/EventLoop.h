#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

/*
 *  reactor 模型的循环部分
 */

namespace netlib
{

class Chnnel;
class Epoller;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
};

}

#endif