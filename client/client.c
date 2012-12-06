#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "tcp.h"

#define PORT "8888" // the default port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once 

int main(int argc, char *argv[])
{
    int csock, numbytes;  
    char buf[MAXDATASIZE];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    csock = tcp_connect(argv[1], PORT);
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









