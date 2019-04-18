//
// Created by zmus on 18/04/19.
//

#ifndef TINYSPY_EXECUTOR_H
#define TINYSPY_EXECUTOR_H


#include "../Connection/ConnectionCollector.h"
#include "../Pipes/Pipe.h"

class Executor {
private:
    ConnectionCollector& connCollector;
    Pipe& pipeline;
    std::vector<pthread_t> executionThreads;
public:
    Executor(Pipe nPipeline, ConnectionCollector& nConnCollector)
        : pipeline(nPipeline),
        connCollector(nConnCollector),
        executionThreads()
    {}
    void listenOnPipe();

};


#endif //TINYSPY_EXECUTOR_H
