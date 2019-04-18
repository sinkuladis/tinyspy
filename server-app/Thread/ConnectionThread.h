//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONTHREAD_H
#define TINYSPY_CONNECTIONTHREAD_H

#include <mutex>
#include <functional>
#include "../Connection/ConnectionCollector.h"
#include "../Socket/Socket.h"

class ConnectionThread {
private:
    ConnectionCollector connCollector;
    Socket listenSock;
    fd_set listened_fds;
    int console_fd;
    int max_pending_conns;
    pthread_t thread_id;
    std::mutex run_mutex;
    static void* conn_routine(void*);
    int initListenedFdSet();

public:
    void initListeningSocket(sockaddr_in server_addr);
    ConnectionThread(int n_console_fd, int n_max_connections)
        : console_fd(n_console_fd), max_pending_conns(n_max_connections), listenSock(Socket())
        {}
        ConnectionCollector& getConnCollector() {return std::ref(connCollector);}
    void run();
};


#endif //TINYSPY_CONNECTIONTHREAD_H
