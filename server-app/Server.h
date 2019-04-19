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

class Server {
private:
    Pipe consoleToConnectionPipe;
    //Pipe connectionToSerializerPipe;
    //Pipe serializerToExecutorPipe;
    Pipe consoleToExecutorPipe;
    Pipe connectionToExecutorPipe;
    ConnectionThread connectionThread;
    ConnectionCollector connCollector;

    //Serializer serializer;
    ExecutorThread executorMainThread;

    sockaddr_in server_addr;
public:
    Server(int listening_port, int max_pend_conn)
        : consoleToConnectionPipe(Pipe()),
          //connectionToSerializerPipe(Pipe()),
          //serializerToExecutorPipe(Pipe()),
          connectionToExecutorPipe(Pipe()),
          consoleToExecutorPipe(Pipe()),
          connCollector(),
          connectionThread( std::ref(consoleToConnectionPipe), std::ref(connectionToExecutorPipe) , max_pend_conn, std::ref(connCollector)),
          executorMainThread(std::ref(connectionToExecutorPipe), std::ref(consoleToExecutorPipe), std::ref(connCollector))
    {
        server_addr = {
                .sin_family=AF_INET,
                .sin_port=htons(listening_port)
        };
    }
    void start();
    int getListeningPort() { return ntohs(server_addr.sin_port); }
};


#endif //TINYSPY_SERVER_H
