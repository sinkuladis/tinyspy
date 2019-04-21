//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONTHREAD_H
#define TINYSPY_CONNECTIONTHREAD_H

#include <mutex>
#include <functional>
#include "../Connection/ConnectionCollector.h"
#include "../Socket/Socket.h"
#include "Thread.h"

class ConnectionThread : public Thread {
private:
    ConnectionCollector& connCollector;
    Socket listenSock;
    fd_set listened_fdset;
    fd_set exception_fdset;
    Pipe& executorPipe;
    int max_pending_conns;
    std::mutex run_mutex;
    static void* conn_routine(void*);
    int initFdSets();

public:
    void initListeningSocket(sockaddr_in server_addr);
    ConnectionThread(Pipe& nConsolePipe, Pipe& nExecutorPipe, int n_max_connections, ConnectionCollector& newConnColl)
        : Thread(nConsolePipe),
        executorPipe(nExecutorPipe),
        max_pending_conns(n_max_connections),
        listenSock(Socket()),
        connCollector(newConnColl)
        {}

    void run() override;
    void join() override;
};


#endif //TINYSPY_CONNECTIONTHREAD_H
