//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_SOCKET_H
#define TINYSPY_SOCKET_H

#include <netinet/in.h>
class Socket {
private:
    static const int BASIC_SLEEP = 3;
    int sock_fd;
    int domain;
    int type;
    sockaddr_in sock_addr;
public:
    Socket() : domain(-1), type(-1), sock_fd(-1) {}
    Socket initialize(int domain=AF_INET, int type=SOCK_STREAM);
    void bind(sockaddr_in&);
    void listen(int);
    void shut();
    void setNonblocking();

    int read(char *buf, int nbytes = 1024);
    int write(char *output, int nbytes);

    int getSockFd() const;
    void connect();
    Socket accept(int new_sock_domain=AF_INET, int new_sock_type=SOCK_STREAM);
};


#endif //TINYSPY_SOCKET_H
