//
// Created by zmus on 16/04/19.
//

#include "Socket.h"
#include<iostream>
#include <unistd.h>
#include <fcntl.h>
#include <functional>


//nie!!! wrzucac do destruktora
void Socket::shut() { //albo w destruktorze
    std::cout << "Zamykanie socketu: " << sock_fd << std::endl;
    shutdown(sock_fd, SHUT_RDWR);
}

int Socket::read(int32_t *num) {
    return ::read(sock_fd, num, sizeof(int32_t));
}

int Socket::write(const int32_t *num) {
    return ::write(sock_fd, num, sizeof(int32_t));
}

int Socket::getSockFd() const {
    return sock_fd;
}

int Socket::write(const char *inbuf, int nbytes) {
    return RWOperation::write(sock_fd, inbuf, nbytes);
}

int Socket::read(char* outbuf, int nbytes) {
    return RWOperation::read(sock_fd, outbuf, nbytes);
}



