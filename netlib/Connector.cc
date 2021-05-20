#include "./Connector.h"
#include "./Logging.h"
#include "./EventLoop.h"
#include "./Chnnel.h"
#include "./SockFunc.h"

#include <assert.h>

using namespace netlib;

Connector::Connector(EventLoop *loop, const SockAddr &addr)
    : _loop(loop),
      _addr(addr),
      _connect(false),
      _state(cDisconnected)
{
}

Connector::~Connector() {
    //
}

void Connector::start() {
    _connect = true;
    _loop->runInLoop(std::bind(&Connector::startInLoop, this));
}

void Connector::startInLoop() {
    _loop->assertInLoopThread();
    assert(_state == cDisconnected);
    if(_connect) {
        /// 发起连接请求
        connect();
    }
    else {
        LOG_DEBUG << "do not connect";
    }
}

void Connector::connect() {
    int sockFd = netlib::createNoblockingOrDie(_addr.getFamily());
    int ret = netlib::connect(sockFd, _addr.getSockAddr());

    int savedError = (ret == 0) ? 0 : errno;
    switch(savedError) {
        case 0:
        default:
            LOG_SYSERR << "Unexpect error in Connector::startInLoop " << savedError;
            netlib::close(sockFd);
            break;
    }
}
