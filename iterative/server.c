#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "8888"  // the default port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//caller has to free the pointer returned by the function
char* get_ip_addr(struct sockaddr_storage* ia) {
    char* s = malloc(INET6_ADDRSTRLEN);
    if (s == NULL) {
        return NULL;
    }
    
    inet_ntop(ia->ss_family, get_in_addr((struct sockaddr *)ia),
              s, INET6_ADDRSTRLEN);
    return s; 
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

int start_server(char* port, int backlog) {
    char* ip;
    int ssock, csock;
    struct sigaction sa;
    struct sockaddr_storage ca;
    
    if ((ssock = tcp_bind(port)) == -1) {
        return -1;
    }

    if (tcp_listen(ssock, backlog) == -1) {
        return -1;
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    printf("server: waiting for connections on port %s ...\n", port);

    while(1) {  // main accept() loop
        csock = tcp_accept(ssock, &ca);

        if (csock == -1) {
            perror("accept");
            continue;
        }

        ip = get_ip_addr(&ca);
        if (ip) {
            printf("server: got connection from %s\n", ip);
            free(ip);
        }
        else {
            printf("server: failed to get ip address of the client!");
        }

        if (send(csock, "Hello, world!", 13, 0) == -1) {
            perror("send");
        }

        close(csock);
    }
}

void print_usage(char* app) {
    printf("usage : ./%s <port> <backlog> \
\t<port> - the listening port. (default : 8888). \
\t<backlog> - the number of pending connections the server will hold. \
");
}

//returns -1 on error
short parse_port(char* s) {
    char* tail;
    long int r = strtol(s, &tail, 10);

    if (tail != NULL) {
        return (short)-1;
    }

    return (short)r;
}

int main(int argc, char* argv[])
{
    char* port;
    int backlog;

    if (argc == 2) {
        if (argv[1] == "-h") {
            print_usage(argv[0]);
            exit(0);
        }

        port = argv[1];
    }
    else {
        //set up the defaults
        port = PORT;
        backlog = BACKLOG;
    }

    //this call will block
    start_server(port, backlog);

    //we will never reach here
    printf("server is exiting ...\n");
    
    return 0;
}

