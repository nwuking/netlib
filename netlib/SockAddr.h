#ifndef SOCKADDR_H_
#define SOCKADDR_H_

#include <netinet/in.h>

#include <string>

/// 封装套接字地址结构

/// struct in_addr {
///     in_addr_t s_addr;                               /// 32位的IPv4地址，网络字节序
/// };

/// struct sockaddr_in {
///     uint8_t sin_len;                                    
///     sa_family_t sin_family;                         /// AF_INET
///     in_port_t sin_port;                             /// 16位端口号，网络字节序
///     struct in_addr sin_addr;                        /// IPv4的地址，网络字节序
///     char  zeros[8];
/// };

namespace netlib
{

class SockAddr
{
public:
    SockAddr() = default;
    explicit SockAddr(uint16_t port, bool loopback = false);                 /// 给定一个port
    SockAddr(std::string ip, uint16_t port);                                 /// 给定ip and port
    SockAddr(const struct sockaddr_in &addr) 
      : _addr(addr)
    {
        /// 给定一个套接子地址结构
    }                        
    /// ~SockAddr();        a trivial destructor

    const  struct sockaddr_in* getSockAddr() const{
        return &_addr;
    }

    sa_family_t getFamily() const {
        return _addr.sin_family;
    }
    
    void setSockAddrInet(struct sockaddr_in &addr) {
        _addr = addr;
    }

    std::string toIpPort() const;

private:
    struct sockaddr_in _addr;
};

}

#endif