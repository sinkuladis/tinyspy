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
    std::unique_lock<std::mutex> lock(mutex);
    FD_ZERO(listen);
    FD_ZERO(exc);
    int max_fd = -1, fd;
    for( auto& conn : connections ){
        fd = conn.second.getSock().getSockFd();
        FD_SET( fd, listen );
        FD_SET( fd, exc);
        max_fd = fd > max_fd ? fd : max_fd;
    }
    return max_fd;
}

ConnectionManager::~ConnectionManager() {
    shutdownAllNow();
}

void ConnectionManager::shutdownNow(int connection_id) {
    std::unique_lock<std::mutex> lock(mutex);
    try {
        Connection &conn = connections.at(connection_id);
        conn.getRequestQueue().shutdownConnectionNow();
    }catch (std::out_of_range e) {
        std::cerr << "No conneciton under id "<<connection_id<<std::endl;
    }
}

void ConnectionManager::shutdownAllNow() {
    std::unique_lock<std::mutex> lock(mutex);
    for(auto it = connections.begin() ; it != connections.end() ; ){
        Connection& conn = it->second;
        conn.getRequestQueue().shutdownConnectionNow();
    }
}

void ConnectionManager::unregister(int id) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.erase(id);
}

void ConnectionManager::collect(Connection& me) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.insert({me.getId(), me});
}

void ConnectionManager::readAll(fd_set *listen, fd_set *exc) {
    std::unique_lock<std::mutex> lock(mutex);
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
}