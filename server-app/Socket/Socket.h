//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_SOCKET_H
#define TINYSPY_SOCKET_H

#include <netinet/in.h>
#include "../basic/RWOperation.h"

class ListeningSocket;
class Socket {
    friend class ListeningSocket;
private:
    int domain;
    int type;
    int sock_fd;
    sockaddr_in sock_addr;
public:
    Socket()
    : sock_fd(-1),
    domain(-1),
    type(-1)
    {}

    void shut();
    int read(void *outbuf, int nbytes);
    int read(int32_t *num);
    int write(const void *inbuf, int nbytes);
    int write(const int32_t *num);
    int getSockFd() const;

};


#endif //TINYSPY_SOCKET_H
