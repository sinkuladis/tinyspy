//
// Created by zmus on 16/04/19.
//

#include <functional>
#include <sys/select.h>
#include <unordered_map>
#include "ConnectionManager.h"
#include "../Console/CommandCode.h"
#include "executor_args.h"

int ConnectionManager::getConnectionsFdSet(fd_set* listen, fd_set* exc) {
    mutex.lock();
    FD_ZERO(listen);
    FD_ZERO(exc);
    int max_fd = -1, fd;
    for( auto& conn : connections ){
        fd = conn.second.getSock().getSockFd();
        FD_SET( fd, listen );
        FD_SET( fd, exc);
        max_fd = fd > max_fd ? fd : max_fd;
    }
    mutex.unlock();
    return max_fd;
}

ConnectionManager::~ConnectionManager() {
    shutdownAllNow();
}

void ConnectionManager::shutdownNow(int connection_id) {
    mutex.lock();
    try {
        Connection &conn = connections.at(connection_id);
        conn.getRequestQueue().shutdownConnectionNow();
    }catch (std::out_of_range e) {
        std::cerr << "No conneciton under id "<<connection_id<<std::endl;
    }
    mutex.unlock();
}

void ConnectionManager::shutdownAllNow() {
    mutex.lock();
    for(auto it = connections.begin() ; it != connections.end() ; ){
        Connection& conn = it->second;
        conn.getRequestQueue().shutdownConnectionNow();
    }
    mutex.unlock();
}

void ConnectionManager::unregister(int id) {
    mutex.lock();
    connections.erase(id);
    mutex.unlock();
}

void ConnectionManager::collect(Connection& me) {
    mutex.lock();
    connections.insert({me.getId(), me});
    mutex.unlock();
}

void ConnectionManager::readAll(fd_set *listen, fd_set *exc) {
    mutex.lock();
    for(auto it = connections.begin() ; it != connections.end() ; ++it) {
        Connection& conn = it->second;
        int conn_sock_fd = conn.getSock().getSockFd();
        if(FD_ISSET(conn_sock_fd, exc)) {
            //TODO handle socket exception
        }
        if(FD_ISSET(conn_sock_fd, listen)) {
            conn.readReceivedData();
        }
    }
    mutex.unlock();
}
