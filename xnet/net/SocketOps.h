//
// Created by zhangkuo on 17-7-30.
// This is internal header file ,you should not include this

#ifndef XNET_SOCKETOPS_H
#define XNET_SOCKETOPS_H

#include <arpa/inet.h>
#include <netinet/tcp.h>

namespace xnet
{
namespace sockets
{

void fromIpPort(const char* ip, uint16_t port, sockaddr_in* addr);
void fromIpPort(const char* ip, uint16_t port, sockaddr_in6* addr);
void toIp(char* buf, size_t size, const sockaddr* addr);
void toIpPort(char* buf, size_t size, const sockaddr* addr);

const sockaddr*     sockaddr_cast(const sockaddr_in* addr);
const sockaddr*     sockaddr_cast(const sockaddr_in6* addr);
sockaddr*           sockaddr_cast(sockaddr_in* addr);
sockaddr*           sockaddr_cast(sockaddr_in6* addr);
const sockaddr_in*  sockaddr_in_cast(const sockaddr* addr);
const sockaddr_in6* sockaddr_in6_cast(const sockaddr* addr);

int     createNonblockingOrDie(sa_family_t family);
int     connect(int sockfd, const sockaddr* addr);
void    bindOrDie(int sockfd, const sockaddr* addr);
void    listenOrDie(int sockfd);
int     accept(int sockfd, sockaddr_in6* addr);
ssize_t read(int sockfd, void* buf, size_t count);
ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);
ssize_t write(int sockfd, const void* buf, size_t count);
void    close(int sockfd);
void    shutdownWrite(int sockfd);

void setNonBlockAndCloseOnExec(int sockfd);
void setTcpNoDelay(int sockfd, bool on);
void setReuseAddr(int sockfd, bool on);
void setReusePort(int sockfd, bool on);
void setKeepAlive(int sockfd, bool on);

bool                getTcpInfo(int sockfd, tcp_info* tcpInfo);
int                 getSocketError(int sockfd);
struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);
bool                isSelfConnect(int sockfd);

}
}

#endif //XNET_SOCKETOPS_H
