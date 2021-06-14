#include "netlib/net/Socket.h"
#include "netlib/net/SockFunc.h"
#include "netlib/net/SockAddr.h"
#include "netlib/base/Logging.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>

using namespace netlib;
/*
struct tcp_info {
    uint32_t tcpi_state;       // tcp state: TCP_SYN_SEND,TCP_SYN_RECV,TCP_CLOSE etc.
    uint32_t tcpi_ca_state;    // 
    uint32_t tcpi_retransmits; // 当前待重传的包数，重传完毕后清0
    uint32_t tcpi_probes;      // 持续定时器发送且未确认的段数
    uint32_t tcpi_backoff;     // 用来计算持续定时器的下一个设计值的指数的退避算法指数，传送超时会递增
    uint32_t tcpi_option;      // tcp 头部是否包含：扩展因子、时间戳、MSS等
    uint32_t tcpi_snd_wscale:4;
    uint32_t tcpi_rcv_wscale:4;  // 扩展因子数值
    uint32_t tcpi_delivery_rate_app_limited:1;  // 限速标志

    uint32_t tcpi_rto;           // 重传超时时间
    uint32_t tcpi_ato;           // 用来延时确认的估值，在收到TCP报文的时候
                                // 会根据上次和本次接收的时间间隔来调整
                                // 在设置确认延迟定时器时会根据条件修改
    uint32_t tcpi_snd_mss;      // 
    uint32_t tcpi_rcv_mss;
    uint32_t tcpi_unacked;      // 未确认的数据段数
    uint32_t tcpi_sacked;       //2个含义：在server端listen()接收连接的数量;sack的数量
    uint32_t tcpi_lost;         // 本端发送出去丢失的报文数，重传后清0
    uint32_t tcpi_retrans;      // 重传且未确认的数据段数
    uint32_t tcpi_fackets;

    uint32_t tcpi_last_date_send;   // 当前时间，最后一个包发送的时间/seconds
    uint32_t tcpi_last_ack_send;
    uint32_t tcpi_last_date_recv;
    uint32_t tcpi_last_ack_recv;

    uint32_t tcpi_pmtu;         // 最后一次更新的路径MTU
    uint32_t tcpi_rcv_ssthread; // 当前接收窗口的大小
    uint32_t tcpi_rtt;
    uint32_t tcpi_rttvar;       // RTT的平均变差，越大，说明RTT抖动越大
    uint32_t tcpi_snd_ssthread; // 拥塞窗口慢开始阀值
    uint32_t tcpi_cwnd;         // 拥塞窗口的大小
    uint32_t tcpi_advmss;       // 本端mss的上限
    uint32_t tcpi_reordering;   // 没有丢包时，可以重新排序的包数
    uint32_t tcpi_rcv_rtt;      // 作为接收端，测试出的RTT的值
    uint32_t tcpi_rcv_space;    // 当前接收缓存的大小
    uint32_t tcpi_total_retrans;    //总重传的包数
    uint32_t tcpi_pacing_rate;       // 发送速率
    uint32_t tcpi_max_pacing_rate;  //最大发送速率，默认unlimit,可以通过SO_MAX_PACING_RATE来设置
    uint32_t tcpi_bytes_ack;
    uint32_t tcpi_bytes_recvived;
    uint32_t tcpi_seds_out;
    uint32_t tcpi_seds_in;
    uint32_t tcpi_notsends_bytes;
    uint32_t tcpi_min_rtt;
    uint32_t tcpi_data_seds_out;
    uint32_t tcpi_data_seds_in;
    uint32_t tcpi_delivery_rate;
    uint32_t tcpi_bus_time;
    uint32_t tcpi_rwnd_limited;
    uint32_t tcpi_sndbuf_limited;
};
*/

Socket::~Socket() {
    netlib::close(_sockFd);
}

bool Socket::getTcpInfo(struct tcp_info *tcpi) const {
    socklen_t len = sizeof(*tcpi);
    ::bzero(tcpi, len);
    return ::getsockopt(_sockFd, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::getTcpInfoString(char *buf, size_t len) const {
    struct tcp_info tcpi;
    bool ok = getTcpInfo(&tcpi);
    if(ok) {
        snprintf(buf, len, "unrecovered=%u "
                "rto=%u ato=%u snd_mss=%u rec_mss=%u "
                "lost=%u retrans=%u rtt=%u rttvar=%u "
                "ssthread=%u cwnd=%u total_retrans=%u",
                tcpi.tcpi_rto, tcpi.tcpi_ato, 
                tcpi.tcpi_snd_mss, tcpi.tcpi_rcv_mss,
                tcpi.tcpi_lost, tcpi.tcpi_retrans,
                tcpi.tcpi_rtt, tcpi.tcpi_rttvar,
                tcpi.tcpi_snd_ssthresh,
                tcpi.tcpi_snd_cwnd,
                tcpi.tcpi_total_retrans);
    }
    return ok;
}

void Socket::bindSockAddr(const SockAddr &localAddr) {
    /// 将地址绑定套接字
    netlib::bind(_sockFd, (const struct sockaddr*)(localAddr.getSockAddr()));
}

void Socket::listen() {
    /// 
    netlib::listen(_sockFd);
}

int Socket::accept(SockAddr *peerAddr) {
    struct sockaddr_in addr;
    ::bzero(&addr, sizeof addr);
    int newFd = netlib::accept(_sockFd, &addr);
    if(newFd >= 0) {
        peerAddr->setSockAddrInet(addr);
    }
    return newFd;
}

void Socket::shutdownWrite() {
    /// 关闭写端，只能接收数据，不能发送数据
    netlib::shutdownWrite(_sockFd);
}

void Socket::setKeepAlive(bool on) {
    int val = on ? 1 : 0;
    ::setsockopt(_sockFd, SOL_SOCKET, SO_KEEPALIVE, &val, static_cast<socklen_t>(sizeof val));
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    int ret =  ::setsockopt(_sockFd, SOL_SOCKET, SO_REUSEPORT,
                 &optval, static_cast<socklen_t>(sizeof(optval)));

    if(ret < 0 && on) {
        LOG_SYSERR << "SO_REUSEPORT failed!";
    }
}

/*
void Socket::setTcpNoDelay(bool on) {
    int val = on ? 1 : 0;
    ::setsockopt(_sockFd, IPPROTO_TCP, TCP_NODELAY, &val, static_cast<socklen_t>(sizeof val));
}
*/