//
// Created by zmus on 16/04/19.
//

#include "Socket.h"
#include<iostream>
#include <unistd.h>
#include <fcntl.h>
#include <functional>

Socket Socket::initialize(int domain, int type) {
    int currTry = 1;
    this->domain = domain; this->type=type;
    do {
        sock_fd = socket( domain , type, 0 );
        if( sock_fd < 0 ) {

            perror( "ERROR while creating a socket in try:");
            sleep(BASIC_SLEEP);
        }
    } while (sock_fd < 0);
    std::cout << "Socket created: " << sock_fd << std::endl;
}

//nie!!! wrzucac do destruktora
void Socket::shut() { //albo w destruktorze
    std::cout << "Zamykanie socketu: " << sock_fd << std::endl;
    shutdown( sock_fd, SHUT_RDWR );
}

void Socket::bind(sockaddr_in &addr_to_bind) {
    int currTry = 1;
    int bindStatus;

    do {
        bindStatus = ::bind( sock_fd, ( struct sockaddr * ) &addr_to_bind, sizeof( addr_to_bind ) );
        if( bindStatus < 0 ) {
            perror( "ERROR while binding socket: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (bindStatus < 0);
    sock_addr = addr_to_bind;
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
            perror( "ERROR while listening: " + currTry++ ); //FIXME + to nie jest append, jak w Javie XD
            sleep(BASIC_SLEEP);
        }
    } while (listenStatus < 0);
    std::cout << "Listening." << std::endl;
}

int Socket::getSockFd() const {
    return sock_fd;
}

Socket Socket::accept(int new_sock_domain, int new_sock_type) {
    Socket newSock;
    socklen_t addr_len = sizeof( newSock.sock_addr );
    int new_sockfd = ::accept(sock_fd,( struct sockaddr * ) &newSock.sock_addr, &addr_len );

    if(new_sockfd <= 0 ) {
        //TODO error
    }else {
        newSock.sock_fd = new_sockfd;
        newSock.domain=new_sock_domain;
        newSock.type=new_sock_type;
    }
    return newSock;
}

int Socket::write(char *inbuf, int nbytes) {
    return RWOperation::write(sock_fd, inbuf, nbytes);
}

int Socket::read(char* outbuf, int nbytes) {
    return RWOperation::read(sock_fd, outbuf, nbytes);
}


