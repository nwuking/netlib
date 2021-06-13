#include "netlib/net/SockAddr.h"
#include "netlib/net/SockFunc.h"
#include "netlib/base/Logging.h"

#include <assert.h>
#include <string.h>
#include <netdb.h>

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

std::string SockAddr::toIpPort() const {
    char buf[64]="";
    netlib::toIpPort(buf, sizeof buf, getSockAddr());
    return buf;
}

std::string SockAddr::toIp() const {
    char buf[64]="";
    netlib::toIp(buf, sizeof buf, getSockAddr());
    return buf;
}

uint16_t SockAddr::tpPort() const {
    return netlib::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuf[64*1024];

bool SockAddr::resolve(std::string hostname, SockAddr *addr) {
    assert(addr != nullptr);
    struct hostent hent;
    struct hostent *he = NULL;
    int herron = 0;
    ::bzero(&hent, sizeof hent);

    int ret = ::gethostbyname_r(hostname.c_str(), &hent, t_resolveBuf,
                                sizeof t_resolveBuf, &he, &herron);
    if(ret == 0 && he != nullptr) {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        addr->_addr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else {
        if(ret) {
            LOG_SYSERR << "SockAddr::resolve";
        }
        return false;
    }
}