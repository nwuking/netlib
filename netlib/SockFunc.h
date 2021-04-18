#ifndef SOCKFUNC_H_
#define SOCKFUNC_H_

/*
 *  本文件封装与socket相关的一些函数
 */

#include <string>

struct in_addr;

namespace netlib
{

void hostToNetwork32(struct in_addr *addr);
void hostToNetwork16(uint16_t *port, uint16_t por);
void fromIp(struct in_addr *addr, const char *ip);

void close(int fd);

}

#endif