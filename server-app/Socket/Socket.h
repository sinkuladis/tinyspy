//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_SOCKET_H
#define TINYSPY_SOCKET_H

#include <netinet/in.h>
#include "../basic/RWOperation.h"



class ListeningSocket;
class Socket : protected RWOperation {
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
    int read(char* outbuf,int nbytes);
    int write(char* inbuf, int nbytes);
    int getSockFd() const;

};


#endif //TINYSPY_SOCKET_H
