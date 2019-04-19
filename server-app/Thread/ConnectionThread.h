//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONTHREAD_H
#define TINYSPY_CONNECTIONTHREAD_H

#include <mutex>
#include <functional>
#include "../Connection/ConnectionCollector.h"
#include "../Socket/Socket.h"
#include "../Pipes/Pipe.h"

class ConnectionThread {
private:
    ConnectionCollector& connCollector;
    Socket listenSock;
    fd_set listened_fds;
    Pipe& consolePipe;
    Pipe& executorPipe;
    int max_pending_conns;
    pthread_t thread_id;
    std::mutex run_mutex;
    static void* conn_routine(void*);
    int initListenedFdSet();

public:
    void initListeningSocket(sockaddr_in server_addr);
    ConnectionThread(Pipe& nConsolePipe, Pipe& nExecutorPipe, int n_max_connections, ConnectionCollector& newConnColl)
        : consolePipe(nConsolePipe), executorPipe(nExecutorPipe), max_pending_conns(n_max_connections), listenSock(Socket()), connCollector(newConnColl)
        {}
    void run();
    void join();
};


#endif //TINYSPY_CONNECTIONTHREAD_H
