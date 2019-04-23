//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONMANAGER_H
#define TINYSPY_CONNECTIONMANAGER_H

#include <mutex>
#include <vector>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <unordered_map>
#include "Connection.h"
#include "../Thread/ExecutorThread.h"

class ExecutorThread; //forward declaration

class ConnectionManager{
private:
    std::mutex mutex;
    std::unordered_map<int,ExecutorThread&> connectionRequestExecutionThreads;
    void shutdownAll();

public:
    ConnectionManager()=default;
    ~ConnectionManager();

    int getConnectionsFdSet(fd_set*);
    std::vector<int> getConnectionDescriptors();

    void addConnection(Socket &listenSock);
    Connection& getConnection(int conn_id);
    void shutdownConnection(int);

    void enter();
    void leave();

    //
    void notifyAll(int commandCode);

    //these two will make the structure of NetworkThread and ConsoleHandler easier since there will be no need to store
    //info about all the pipes for each execution thread inside objects of those 2 classes. They'll just make use of the pipes storen inside ExecutionThreads

    Pipe& getConsolePipe(int connection_id);
    Pipe& getNetworkPipe(int connection_id);

};


#endif //TINYSPY_CONNECTIONMANAGER_H
