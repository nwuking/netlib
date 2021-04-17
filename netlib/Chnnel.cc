#include "./Chnnel.h"


using namespace netlib;

Chnnel::Chnnel(EventLoop *loop, int fd)
    : _ownLoop(loop),
      _fd(fd),
      _events(0),
      _revevts(0)
{
}

Chnnel::~Chnnel() {
}


void Chnnel::handleEvent() {
  /// 事件分发
  
}