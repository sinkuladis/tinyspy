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

#define MAX_MSG_LEN 4096
#define SERWER_PORT 8888
#define SERWER_IP "127.0.0.1"
#define MAX_CONNECTION 2
#define MAX_TRY 3
#define BASIC_SLEEP 2

void shut(int status, int socket) {
    std::cout << "Closing socket " << socket << std::endl;
    shutdown( socket, SHUT_RDWR );
    exit (status);
}

int createMainSocket(int &mainSocket) {
    int currTry = 1;
    do {
        mainSocket = socket( AF_INET, SOCK_STREAM, 0 );
        if( mainSocket < 0 ) {
            perror( "ERROR while creating main socket:" + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && mainSocket < 0);

    if ( mainSocket < 0 ) {
        perror( "ERROR while creating main socket. Shutdown" );
        exit(1);
    }
    else
        std::cout << "Socket created: " << mainSocket << std::endl;
    return mainSocket;

}

struct sockaddr_in createServer() {
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
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

    if ( bindStatus < 0 )
        shut(1, socket);
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

    if ( listenStatus < 0 )
        shut(2, socket);
    else
        std::cout << "Listening." << std::endl;
}
