//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_SERVER_H
#define TINYSPY_SERVER_H


#include <netinet/in.h>
#include <functional>
#include "Thread/ConnectionThread.h"
#include "Pipes/Pipe.h"
#include "Thread/ExecutorThread.h"
#include "Console/ConsoleHandler.h"

class Server {
private:
    Pipe consoleToConnectionPipe;
    //Pipe connectionToSerializerPipe;
    //Pipe serializerToExecutorPipe;
    Pipe consoleToExecutorPipe;
    Pipe connectionToExecutorPipe;
    ConnectionThread connectionThread;
    ConnectionCollector connCollector;
    ConsoleHandler consoleHandler;

    ExecutorThread executorMainThread;

    sockaddr_in server_addr;
public:
    Server(int listening_port, int max_pend_conn);
    void start();
    int getListeningPort();
};


#endif //TINYSPY_SERVER_H
