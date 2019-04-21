//
// Created by zmus on 18/04/19.
//

#ifndef TINYSPY_EXECUTORTHREAD_H
#define TINYSPY_EXECUTORTHREAD_H

#include<thread>
#include "Thread.h"
#include "../Connection/ConnectionCollector.h"

class ExecutorThread : public Thread {
private:
    ConnectionCollector& connCollector;
    Pipe& connectionPipe;
    std::thread mainThread;
    fd_set listened_pipes;
    fd_set exception_pipes;

public:
    ExecutorThread(Pipe& nConnectionPipe, Pipe& nConsolePipe, ConnectionCollector& nConnCollector)
        : Thread(nConsolePipe),
        connectionPipe(nConnectionPipe),
        connCollector(nConnCollector)
    {}
    void listenForRequests();
    void handleRequest(); //main routine
    void initFdSets();

    void run() override;
    void join() override;
};


#endif //TINYSPY_EXECUTORTHREAD_H
