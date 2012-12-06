#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "tcp.h"

#define MAXDATASIZE 100 // max number of bytes we can get at once 

void help() {
    printf("client: invalid hostname or port!\n");
}

int main(int argc, char *argv[])
{
    char buf[MAXDATASIZE];
    char* hostname = NULL;
    int csock, numbytes;  
    char* port = NULL;

    while (1) {
        char c;

        c = getopt (argc, argv, "h:p:");
        if (c == -1) {
            break;
        }

        switch (c) {

        case 'h':
            hostname = optarg;
            break;
            
        case 'p':
            port = optarg;   
            break;
            
        case '?':
        default:
            help();
        }
    }

    if (!hostname || !port) {
        help();
        exit(2);
    }
    
    csock = tcp_connect(hostname, port);
    if (csock < 0) {
        return -1;
    }

    if ((numbytes = recv(csock, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(csock);

    return 0;
}









