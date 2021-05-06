#include "./SockAddr.h"
#include "./SockFunc.h"

#include <string.h>

using namespace netlib;

SockAddr::SockAddr(uint16_t port, bool loopback) {
    /// loopback默认为false
    /// loopback=true时，表示使用"127.0.0.1"
    ::bzero(&_addr, sizeof _addr);
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;

    netlib::hostToNetwork32(&_addr.sin_addr);
    netlib::hostToNetwork16(&_addr.sin_port, port);
}

SockAddr::SockAddr(std::string ip, uint16_t port) {
    /// 指定ip地址和端口号
    ::bzero(&_addr, sizeof _addr);
    _addr.sin_family = AF_INET;
    netlib::hostToNetwork16(&_addr.sin_port, port);
    netlib::fromIp(&_addr.sin_addr, ip.c_str());
}
