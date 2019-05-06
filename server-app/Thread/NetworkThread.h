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
#include "../Socket/ListeningSocket.h"


class NetworkThread : public Thread {
private:
    ConnectionManager& connMgr;
    ListeningSocket listenSock;
    fd_set listened_fdset;
    fd_set exception_fdset;
    Pipe& consolePipe;
    int max_pending_conns;
    std::mutex run_mutex;
    static void* conn_routine(void*);
    int initFdSets();
    int listening_port;

    void acceptNewConnection();
public:
    NetworkThread(Pipe& nConsolePipe, int n_max_connections, ConnectionManager& newConnColl, int port)
    : Thread(),
      consolePipe(nConsolePipe),
      max_pending_conns(n_max_connections),
      listenSock(ListeningSocket(n_max_connections, port)),
      connMgr(newConnColl),
      listening_port(port)
    {}

    ~NetworkThread() {
        listenSock.shut();
    }

    void run() override;
    void join() override;
};


#endif //TINYSPY_CONNECTIONTHREAD_H
