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
    sockaddr_in sock_addr;
public:
    Socket(int domain=AF_INET, int type=SOCK_STREAM);
    void bind(sockaddr_in&);
    void listen(int);
    void shut();
    void setNonblocking();

    int read(void*buffer, int nbytes=1024);
    int write(void*);

    int getSockFd() const;
    void connect();
    Socket& accept(int new_sock_domain=AF_INET, int new_sock_type=SOCK_STREAM);
};


#endif //TINYSPY_SOCKET_H
