#ifndef SOCKFUNC_H_
#define SOCKFUNC_H_

/*
 *  本文件封装与socket相关的一些函数
 */

#include <string>
#include <netinet/in.h>
#include <error.h>
#include <sys/uio.h>
#include <sys/types.h>

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

struct sockaddr_in getLocalAddr(int sockfd);

ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

ssize_t write(int fd, const void *buf, size_t len);

ssize_t read(int fd, void *buf, size_t len);

void shutdownWrite(int fd);

int getSocketError(int fd);


}
#endif