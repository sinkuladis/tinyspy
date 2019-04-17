//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_SERVER_H
#define TINYSPY_SERVER_H


#include <netinet/in.h>
#include <functional>
#include "Thread/ConnectionThread.h"

class Server {
private:
    ConnectionThread connectionThread;
    ConnectionCollector& connCollector;
    sockaddr_in server_addr;
    int console_fd;
public:
    Server(int listening_port, int max_pend_conn, int nconsole_fd) : console_fd(nconsole_fd), connectionThread(console_fd, max_pend_conn), connCollector(std::ref(connectionThread.getConnCollector())) {
        server_addr = {
                .sin_family=AF_INET,
                .sin_port=htons(listening_port)
        };
    }
    void start();
    int getListeningPort() { return ntohs(server_addr.sin_port); }
};


#endif //TINYSPY_SERVER_H
