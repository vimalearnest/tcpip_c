#include "tcp.h"

void* get_in_addr(struct sockaddr_storage* ss)
{
    if (ss->ss_family == AF_INET) {
        return &(((struct sockaddr_in*)ss)->sin_addr);
    }

    return &(((struct sockaddr_in6*)ss)->sin6_addr);
}

char* get_ip_addr(struct sockaddr_storage* ss) {
    size_t addr_len =
        ss->ss_family == AF_INET ?
          INET_ADDRSTRLEN : INET6_ADDRSTRLEN;

    char* s = malloc(addr_len);
    if (s == NULL) {
        return NULL;
    }
    
    inet_ntop(ss->ss_family,
              get_in_addr(ss),
              s,
              addr_len);
    
    return s; 
}

short parse_port(char* s) {
    char* tail;
    long int r = strtol(s, &tail, 10);

    if (tail != NULL) {
        return (short)-1;
    }

    return (short)r;
}

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
            perror("socket");
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

int tcp_bind(char* port) {
    int rv;
    int sockfd;
    int yes = 1;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd,
                       SOL_SOCKET,
                       SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            return -1;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            return -1;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return -1;
    }

    freeaddrinfo(servinfo); // all done with this structure

    return sockfd;
}

int tcp_listen(int ssock, size_t blog) {
    if (listen(ssock, blog) == -1) {
        perror("listen");
        return -1;
    }

    return 1;
}

int tcp_accept(int ssock, struct sockaddr_storage* sa) {
    socklen_t sz = sizeof(struct sockaddr_storage);
    int csock = accept(ssock, (struct sockaddr *)sa, &sz);
    return csock;
}


