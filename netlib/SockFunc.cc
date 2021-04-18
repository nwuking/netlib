#include "./SockFunc.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

using namespace netlib;

void hostToNetwork32(struct in_addr *addr) {
    struct in_addr res;
    int n = ::inet_aton((const char*)addr, &res);
    if(n) {
        ::memcpy(addr, &res, sizeof res);
    }
    else {
        /// n == 0, addr 无效
    }
}

void hostToNetwork16(uint16_t *port, uint16_t por) {
    *port = ::htons(por);
    if(!port) {
        ///  failed
    }
}

void fromIp(struct in_addr *addr, const char *ip) {
    int n = ::inet_aton(ip, addr);
    if(n == 0) {
        /// failed
    }
}


void netlib::close(int fd) {
    int n = ::close(fd);
    if(n < 0) {
        /// n == 0, 表示成功返回
    }
}

