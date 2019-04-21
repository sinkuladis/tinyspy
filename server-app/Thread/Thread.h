//
// Created by zmus on 20/04/19.
//

#ifndef TINYSPY_THREAD_H
#define TINYSPY_THREAD_H

#include <pthread.h>
#include "../Pipes/Pipe.h"

//abstrakcyjna klasa
class Thread {
protected:
    bool running;
    //roboczo pipe z konsoli. Moze stad zniknac w pozniejszych stadiach
    Pipe& consolePipe;
    pthread_t thread_id;
public:
    Thread(Pipe& nConsolePipe)
    : running(false),
    thread_id(),
    consolePipe(nConsolePipe)
    {}
    ~Thread() {}

    //abstrakcyjne metody
    virtual void run() = 0;
    virtual void join() = 0;

    virtual void runCommand(int commandCode);
    virtual void shutdown();
};


#endif //TINYSPY_THREAD_H
