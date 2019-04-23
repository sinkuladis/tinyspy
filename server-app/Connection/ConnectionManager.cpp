//
// Created by zmus on 16/04/19.
//

#include <functional>
#include <sys/select.h>
#include <unordered_map>
#include "ConnectionManager.h"
#include "../Console/CommandCode.h"

void ConnectionManager::addConnection(Socket &listenSock) { //moge zawrzec listenSock w klasie
    Socket newSock = listenSock.accept();

    ExecutorThread *thrd = new ExecutorThread(newSock, std::ref(*this)); //FIXME new, pthready, a po wychodzeniu z petli bede probowal odplac destruktor i w nim zrobie pthread exit
    int connection_id = thrd->getConnection().getId();

    //TODO create a move constructor for Execution thread and then add the moved stuff to the collection?
    connectionRequestExecutionThreads.insert({connection_id, *thrd});

    std::cout<<"Added client #"<<connection_id<<std::endl;
    thrd->run();
}

//monitored method
void ConnectionManager::notifyAll(int commandCode) {
    mutex.lock();

    if(commandCode == SHUTDOWN) {
        shutdownAll();
    }
    else {
        for (auto &id_thread_pair : connectionRequestExecutionThreads) {
            id_thread_pair.second.getConsolePipe().writeInt(commandCode);
        }
    }

    mutex.unlock();
}

int ConnectionManager::getConnectionsFdSet(fd_set *fdsetptr) {
    FD_ZERO(fdsetptr);
    int max_fd = -1, fd;
    for( auto& conn : connectionRequestExecutionThreads ){
        fd = conn.second.getConnection().getSock().getSockFd();
        FD_SET( fd, fdsetptr );
        max_fd = fd > max_fd ? fd : max_fd;
    }
    return max_fd;
}

std::vector<int> ConnectionManager::getConnectionDescriptors() {
    std::vector<int> fdVec;
    for(auto& thrd : connectionRequestExecutionThreads)
        fdVec.push_back(thrd.second.getConnection().getId());
    return fdVec;
}

ConnectionManager::~ConnectionManager() {
    mutex.lock();
    shutdownAll();
    mutex.unlock();
}

Connection& ConnectionManager::getConnection(int conn_id) {
    return std::ref(connectionRequestExecutionThreads.at(conn_id).getConnection());
}

void ConnectionManager::enter() { mutex.lock(); }

void ConnectionManager::leave() { mutex.unlock(); }

//powinny dzialac poprawnie, skoro przechowuje w kontenerach referencje na typy, zamiast konkretnych typow
Pipe &ConnectionManager::getConsolePipe(int connection_id) {
    return connectionRequestExecutionThreads.at(connection_id).getConsolePipe();
}

Pipe &ConnectionManager::getNetworkPipe(int connection_id) {
    return connectionRequestExecutionThreads.at(connection_id).getNetworkPipe();
}

void ConnectionManager::shutdownConnection(int connection_id) {
    ExecutorThread& thread = connectionRequestExecutionThreads.at(connection_id);
    connectionRequestExecutionThreads.erase(connection_id);
    thread.shutdown();
    thread.join();
    delete &thread;
}

void ConnectionManager::shutdownAll() {
    for(auto e = connectionRequestExecutionThreads.begin() ; e != connectionRequestExecutionThreads.end() ; ){
        e->second.shutdown();
        e->second.join();
        delete &e->second;
        e = connectionRequestExecutionThreads.erase(connectionRequestExecutionThreads.begin());
    }
}