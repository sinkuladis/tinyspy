//
// Created by zmus on 16/04/19.
//

#include "Socket.h"
#include<iostream>
#include <unistd.h>
#include <functional>

void Socket::shut() {
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

int Socket::write(const void *inbuf, int nbytes) {
    return ::write(sock_fd, inbuf, nbytes);
}

int Socket::read(void *outbuf, int nbytes) {
    return ::read(sock_fd, outbuf, nbytes);
}



