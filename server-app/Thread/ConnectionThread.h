//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONTHREAD_H
#define TINYSPY_CONNECTIONTHREAD_H

#include "../Connection/ConnectionCollector.h"
#include "../Socket/Socket.h"

class ConnectionThread {
private:
    ConnectionCollector &connCollector;
    Socket listenSock;
    fd_set listened_fds;
    int console_fd;
    int max_pending_conns;
    pthread_t thread_id;
    static void* conn_routine(void*);
    int initListenedFdSet();

public:
    void initListeningSocket(sockaddr_in server_addr);
    ConnectionThread(ConnectionCollector& nConnCollector, int n_console_fd, int n_max_connections)
        : connCollector(nConnCollector), console_fd(n_console_fd), max_pending_conns(n_max_connections)
        {}
    void run();


};


#endif //TINYSPY_CONNECTIONTHREAD_H
