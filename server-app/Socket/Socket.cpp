//
// Created by zmus on 16/04/19.
//

#include "Socket.h"
#include<iostream>
#include <unistd.h>
#include <fcntl.h>

Socket::Socket(int domain, int type) {
    int currTry = 1;
    do {
        sock_fd = socket( domain , type, 0 );
        if( sock_fd < 0 ) {
            perror( "ERROR while creating main socket:" + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (sock_fd < 0);
    std::cout << "Socket created: " << sock_fd << std::endl;
}

void Socket::shut() { //albo w destruktorze
    std::cout << "Zamykanie socketu: " << sock_fd << std::endl;
    shutdown( sock_fd, SHUT_RDWR );
}

void Socket::bind(sockaddr_in &addr) {
    int currTry = 1;
    int bindStatus;

    do {
        bindStatus = ::bind( sock_fd, ( struct sockaddr * ) & addr, sizeof( addr ) );
        if( bindStatus < 0 ) {
            perror( "ERROR while binding socket: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (bindStatus < 0);
    std::cout << "Socket binded." << std::endl;
}

void Socket::setNonblocking()
{
    int flags;
    if (-1 == (flags = fcntl(sock_fd, F_GETFL, 0)))
        flags = 0;
    int ret = fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK); //FIXME nie trzymajmy flagi pod intem
}

void Socket::listen(int max_connections) {
    int currTry = 1;
    int listenStatus;
    do {
        listenStatus = ::listen( sock_fd, max_connections );
        if( listenStatus  < 0 ) {
            perror( "ERROR while listening: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (listenStatus < 0);
    std::cout << "Listening." << std::endl;
}

int Socket::read(int nbytes) {
    //TODO read nbytes; if 0 -> break loop and ret 0
    return 1;
}

int Socket::write(void* output) {
    //TODO write the whole output buffer to the socket
    return 1;
}

int Socket::getSockFd() const {
    return sock_fd;
}

int Socket::accept(struct sockaddr_in* sock_addr) {
    socklen_t addr_len;
    int new_sockfd=0;
    if(sock_addr != nullptr){
        addr_len = sizeof( sock_addr );
        new_sockfd = ::accept(sock_fd,( struct sockaddr * ) &sock_addr, &addr_len );
    }
    else
        new_sockfd = ::accept(sock_fd, nullptr, nullptr );
    if(new_sockfd < 0 ) {
        //TODO error
    }
    return new_sockfd;

}


