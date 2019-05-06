//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONTHREAD_H
#define TINYSPY_CONNECTIONTHREAD_H

#include <mutex>
#include <functional>
#include "../Connection/ConnectionManager.h"
#include "../Socket/Socket.h"
#include "Thread.h"

class NetworkThread : public Thread {
private:
    ConnectionManager& connMgr;
    Socket listenSock;
    fd_set listened_fdset;
    fd_set exception_fdset;
    Pipe& consolePipe;
    int max_pending_conns;
    std::mutex run_mutex;
    static void* conn_routine(void*);
    int initFdSets();

    void acceptNewConnection();
public:
    void initListeningSocket(sockaddr_in server_addr);
    NetworkThread(Pipe& nConsolePipe, int n_max_connections, ConnectionManager& newConnColl)
    : Thread(),
      consolePipe(nConsolePipe),
      max_pending_conns(n_max_connections),
      listenSock(Socket()),
      connMgr(newConnColl)
    {}

    ~NetworkThread() {
        listenSock.shut();
    }

    void run() override;
    void join() override;
};


#endif //TINYSPY_CONNECTIONTHREAD_H
