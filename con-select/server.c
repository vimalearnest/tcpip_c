#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 

#include "tcp.h"

#define PORT "8888"  // the default port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int start_server(char* port, int backlog) {
    char* ip;
    socklen_t addrlen;
    int ssock, csock;
    struct sigaction sa;
    struct sockaddr_storage ca;
    
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    int i;

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

    FD_ZERO(&master);  // clear the master and temp sets
    FD_ZERO(&read_fds);

    // add the listener to the master set
    FD_SET(ssock, &master);

    // keep track of the biggest file descriptor
    fdmax = ssock; // so far, it's this one

    while(1) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        
        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == ssock) {
                    // handle new connections
                    addrlen = sizeof(ca);
                    csock = accept(ssock, (struct sockaddr *)&ca, &addrlen);
    
                    if (csock == -1) {
                        perror("accept");
                    } else {
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
            }
        }
    }
}

void print_usage(char* app) {
    printf("usage : ./%s <port> <backlog> \
\t<port> - the listening port. (default : 8888). \
\t<backlog> - the number of pending connections the server will hold. \
");
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

