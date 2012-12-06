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
 *
 *  note:- caller has to free the memory returned 
 *         by the function on success.
 */
char* get_ip_addr(struct sockaddr_storage* ss);

/** get the IPv4 or IPv6 address  
 *
 *  return
 *      error -> NULL
 *      success -> sockaddr_in* or sockaddr_in6*
 */
void* get_in_addr(struct sockaddr_storage* ss);

/** parse the port number from the given string.
 *
 *  return
 *      error -> -1
 *      success -> port
 */
short parse_port(char* s);

/** connect to a host on the given port
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_connect(char* hostname, char* port);

/** bind to the given port 
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_bind(char* port);

/** put the given sock fd in listen mode
 *
 *  return
 *      error -> -1
 *      success -> 1
 */
int tcp_listen(int ssock, size_t blog);

/** accept connection on the given sock fd
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_accept(int ssock, struct sockaddr_storage* sa);

#endif /* __TCP_H_INCLUDED__ */
