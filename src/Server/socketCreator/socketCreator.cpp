#include "socketCreator.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_MSG_LEN 4096
#define SERWER_IP "127.0.0.1"
#define MAX_CONNECTION 2
#define MAX_TRY 3
#define BASIC_SLEEP 2

void shut(int socket) {
    std::cout << "Zamykanie socketu: " << socket << std::endl;
    shutdown( socket, SHUT_RDWR );
}

int createMainSocket() {
    int currTry = 1;
    int newSocket;
    do {
        newSocket = socket( AF_INET, SOCK_STREAM, 0 );
        if( newSocket < 0 ) {
            perror( "ERROR while creating main socket:" + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && newSocket < 0);

    if ( newSocket < 0 ) {
        perror( "ERROR while creating main socket. Shutdown" );
        exit(1);
    }
    else
        std::cout << "Socket created: " << newSocket << std::endl;
    return newSocket;

}

struct sockaddr_in createServer(int port) {
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons( port )
    };
    return server;
}

void bindSocket(int socket, sockaddr_in &server) {
    socklen_t sockLen = sizeof( server );
    int currTry = 1;
    int bindStatus;

    do {
        bindStatus = bind( socket,( struct sockaddr * ) & server, sockLen );
        if( bindStatus < 0 ) {
            perror( "ERROR while binding socket: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && bindStatus < 0);

    if ( bindStatus < 0 ) {
        shut(socket);
        exit(1);
    }
    else
        std::cout << "Socket binded." << std::endl;
}

void startListening(int socket, int maxConnection) {
    int currTry = 1;
    int listenStatus;
    do {
        listenStatus = listen( socket, maxConnection );
        if( listenStatus  < 0 ) {
            perror( "ERROR while listening: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && listenStatus < 0);

    if ( listenStatus < 0 ) {
        shut(socket);
        exit(1);
    }
    else
        std::cout << "Listening." << std::endl;
}

int setNonblocking(int fd)
{
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}     
