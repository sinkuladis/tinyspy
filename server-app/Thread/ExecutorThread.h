//
// Created by zmus on 18/04/19.
//

#ifndef TINYSPY_EXECUTORTHREAD_H
#define TINYSPY_EXECUTORTHREAD_H

#include<thread>
#include "Thread.h"
#include "../Connection/ConnectionManager.h"

class ConnectionManager; //forward declaration

class ExecutorThread : public Thread {
private:
    ConnectionManager& connMgr;
    Connection connection;
    Pipe networkPipe;
    Pipe consolePipe;
    std::thread mainThread;
    fd_set listened_pipes;
    fd_set exception_pipes;

public:
    ExecutorThread(Socket& connectionSocket, ConnectionManager& nMgr)
        : Thread(),
          connMgr(nMgr),
        connection(connectionSocket),
        networkPipe(Pipe()),
        consolePipe(Pipe())
    {}

    ~ExecutorThread() {
        std::cout<<"destroying exe #"<<connection.getId()<<std::endl;
    }

    void listenForRequests();  //main routine
    void handleRequest();
    void initFdSets();

    void run() override;
    void join() override;

    Pipe& getNetworkPipe() { return std::ref(networkPipe); }
    Pipe& getConsolePipe() { return std::ref(consolePipe); }
    Connection& getConnection() {return std::ref(connection); }
};


#endif //TINYSPY_EXECUTORTHREAD_H
