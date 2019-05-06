//
// Created by darek on 01.05.19.
//

#include "ListeningSocket.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <functional>

struct timeval ListeningSocket::initialize(struct timeval time_left, int domain, int type) {

    struct timeval timeout;

    timeout = {
            .tv_sec = 10,
            .tv_usec = 0
    };

    if(status==1) {
        if(time_left.tv_usec!=0||time_left.tv_sec!=0)
        {
            return time_left;
        }
        else {
            this->domain = domain;
            this->type = type;
            sock_fd = socket(domain, type, 0);
            if (sock_fd < 0) {
                perror("ERROR while creating a socket");
            } else {
                std::cout << "Socket created: " << sock_fd << std::endl;
                if(bind()>=0)
                {
                    if(listen(max_connections)>=0)
                    {
                        if(setNonblocking()>=0)
                            status=2;
                    }
                }
            }
        }
    }
    if(status!=1)
    {
        timeout = {
                .tv_sec = 0,
                .tv_usec = 0
        };
        return  timeout;
    }
    return timeout;

}

//nie!!! wrzucac do destruktora
void ListeningSocket::shut() {
    std::cout << "Zamykanie socketu: " << sock_fd << std::endl;
    shutdown( sock_fd, SHUT_RDWR );
}

int ListeningSocket::bind() {
    int bindStatus;
    bindStatus = ::bind( sock_fd, ( struct sockaddr * ) &server_addr, sizeof( server_addr ) );
    if( bindStatus < 0 ) {
        perror( "ERROR while binding socket");
    }
    else {
        std::cout << "Socket binded." << std::endl;
    }
    return bindStatus;
}

int ListeningSocket::setNonblocking()
{
    int flags;
    if (-1 == (flags = fcntl(sock_fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK); //FIXME nie trzymajmy flagi pod intem
}


int ListeningSocket::listen(int max_connections) {
    int listenStatus;
    listenStatus = ::listen( sock_fd, max_connections );
    if( listenStatus  < 0 ) {
        perror( "ERROR while listening: ");
    }
    else  {
        std::cout << "Listening." << std::endl;
    }
    return listenStatus;
}

int ListeningSocket::getSockFd() const {
    return sock_fd;
}

int ListeningSocket::getStatus() const {
    return status;
}

Socket ListeningSocket::accept(int new_sock_domain, int new_sock_type) {
    Socket newSock;
    socklen_t addr_len = sizeof( newSock.sock_addr );
    int new_sockfd = ::accept(sock_fd,( struct sockaddr * ) &newSock.sock_addr, &addr_len );
    if(new_sockfd <= 0 ) {
        perror("cannot create a new socket");
    }else {
        newSock.sock_fd = new_sockfd;
        newSock.domain=new_sock_domain;
        newSock.type=new_sock_type;
    }
    return newSock;
}

