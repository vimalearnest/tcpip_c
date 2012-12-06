#include "tcp.h"

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

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//caller has to free the pointer returned by the function
char* get_ip_addr(struct addrinfo* ai) {
    char* s = malloc(INET6_ADDRSTRLEN);
    if (s == NULL) {
        return NULL;
    }
    
    inet_ntop(ai->ai_family,
              get_in_addr((struct sockaddr *)ai->ai_addr),
              s,
              INET6_ADDRSTRLEN);
    return s; 
}

// returns -1 on error
int tcp_connect(char* hostname, char* port) {
    char* ip;
    int csock;
    int rv;
    struct addrinfo hints, *si, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &hints, &si)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and connect to the first we can
    for(p = si; p != NULL; p = p->ai_next) {
        if ((csock = socket(p->ai_family, p->ai_socktype,
                            p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(csock, p->ai_addr, p->ai_addrlen) == -1) {
            close(csock);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return -1;
    }

    ip = get_ip_addr(p);
    printf("client: connecting to %s ...\n", ip);
    free(ip);

    freeaddrinfo(si); // all done with this structure

    return csock;
}



