//
// Created by zmus on 18/04/19.
//

#ifndef TINYSPY_EXECUTORTHREAD_H
#define TINYSPY_EXECUTORTHREAD_H

#include<thread>
#include "../Connection/ConnectionCollector.h"
#include "../Pipes/Pipe.h"

class ExecutorThread {
private:
    ConnectionCollector& connCollector;
    Pipe& connectionPipe;
    Pipe& consolePipe;
    std::thread mainThread;
    fd_set listened_pipes;
    fd_set exception_pipes;
    std::vector<pthread_t> executionThreads;
public:
    ExecutorThread(Pipe& nConnectionPipe, Pipe& nConsolePipe, ConnectionCollector& nConnCollector)
        : connectionPipe(nConnectionPipe),
        consolePipe(nConsolePipe),
        connCollector(nConnCollector),
        executionThreads()
    {}
    void listenOnPipe();
    void run();
    void join();
    void initFdSets();
};


#endif //TINYSPY_EXECUTORTHREAD_H
