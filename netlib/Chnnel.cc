#include "./Chnnel.h"

#include <poll.h>

using namespace netlib;

const int Chnnel::cNonEvent = 0;
const int Chnnel::cReadEvent = POLLIN | POLLPRI;
const int Chnnel::cWriteEvent = POLLOUT;

Chnnel::Chnnel(EventLoop *loop, int fd)
    : _ownLoop(loop),
      _fd(fd),
      _events(0),
      _revevts(0),
      _addedToLoop(false)
{
}

Chnnel::~Chnnel() {
}


void Chnnel::handleEvent() {
  /// 事件分发
  if(_revevts & (POLLNVAL | POLLERR)) {
    /// POLLNVAL-> 描述符没有引用一个打开的文件
    /// POLLERR -> 出错
    if(_errorCallBack) {
      _errorCallBack();
    }
  }

  if((_revevts & POLLHUP) && !(_revevts & POLLIN)) {
    /// POLLHUP -> 以挂断，不能对该文件符进行写操作，但可以读
    /// 没有数据读取的时候，关闭它
    if(_closeCallBack) {
      _closeCallBack();
    }
  }

  if(_revevts & (POLLIN | POLLPRI | POLLRDHUP)) {
    /// POLLIN -> 可以不阻塞地读取高优先级以外的数据，等效于(POLLRDNORM | POLLRDBAND)
    /// POLLPRI-> 可以不阻塞地读取高优先级数据
    /// POLLRDHUP -> 描述符已挂断，但可读
    if(_readCallBack) {
      _readCallBack();
    }
  }

  if(_revevts & (POLLOUT | POLLWRBAND)) {
    /// POLLOUT -> 可以不阻塞地写普通数据
    /// POLLWRBAND -> 可以不阻塞地写高优先级数据
    if(_writeCallBack) {
      _writeCallBack();
    }   
  }
}

void Chnnel::update() {
  _addedToLoop = true;
  //_ownLoop->updateChnnel();
}