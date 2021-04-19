#ifndef SOCKFUNC_H_
#define SOCKFUNC_H_

/*
 *  本文件封装与socket相关的一些函数
 */

#include <string>
#include <netinet/in.h>

struct in_addr;

namespace netlib
{

void hostToNetwork32(struct in_addr *addr);
void hostToNetwork16(uint16_t *port, uint16_t por);
void fromIp(struct in_addr *addr, const char *ip);

void close(int fd);
void bind(int fd, const struct sockaddr *addr);
void listen(int fd);

int accept(int fd, struct sockaddr_in *addr);

int createSocketFd(sa_family_t family);


}
#endif