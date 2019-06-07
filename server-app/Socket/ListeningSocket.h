//
// Created by darek on 01.05.19.
//

#ifndef TINYSPY_LISTENINGSOCKET_H
#define TINYSPY_LISTENINGSOCKET_H
#include "Socket.h"
#include <netinet/in.h>
#include <sys/time.h>
#include "../basic/RWOperation.h"


class ListeningSocket {
private:
    int domain;
    int type;
    int sock_fd;
    int status;
    int max_connections;
    sockaddr_in server_addr;

public:
    ListeningSocket(int max, int listening_port)
            : sock_fd(-1),
              domain(-1),
              type(-1),
              status(1),
              max_connections(max)

    {
        server_addr = {
                .sin_family=AF_INET,
                .sin_port=htons(listening_port)
        };
    }

    struct timeval initialize(struct timeval time_left, int domain=AF_INET, int type=SOCK_STREAM);
    int bind();
    int listen(int);
    void shut();
    int setNonblocking();
    int getSockFd() const;
    int isReady() const;
    Socket accept(int new_sock_domain=AF_INET, int new_sock_type=SOCK_STREAM);

    void setBroken();
    void setReady();
};

#endif //TINYSPY_LISTENINGSOCKET_H
