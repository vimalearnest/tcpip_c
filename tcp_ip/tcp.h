#ifndef __TCP_H_INCLUDED__
#define __TCP_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/** get the ip address as a string
 *
 *  return
 *      error -> NULL
 *      success -> ip address
 */
char* get_ip_addr(struct addrinfo* ai);

/** get the IPv4 or IPv6 address  
 *
 *  return
 *      error -> NULL
 *      success -> sockaddr_in* or sockaddr_in6*
 */
void* get_in_addr(struct sockaddr *sa);

/** connect to a host on the given port
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_connect(char* hostname, char* port);

#endif /* __TCP_H_INCLUDED__ */
