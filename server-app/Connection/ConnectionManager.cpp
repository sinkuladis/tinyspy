//
// Created by zmus on 16/04/19.
//

#include <functional>
#include <unordered_map>
#include <Exception/ConnectionTerminationException.h>
#include <Pipes/Pipe.h>
#include "ConnectionManager.h"

int ConnectionManager::getConnectionsFdSet() {
    std::unique_lock<std::mutex> lock(mutex);
    FD_ZERO(listened_fdset);
    FD_ZERO(write_fdset);
    FD_ZERO(exception_fdset);
    int max_fd = -1, fd;
    for( auto& item : connections ) {
        Connection& connection = item.second;
        fd = connection.getSock().getSockFd();
        FD_SET(fd, listened_fdset);
        if(connection.isReadyToSend())
            FD_SET(fd, write_fdset);
        FD_SET(fd, exception_fdset);

        max_fd = fd > max_fd ? fd : max_fd;
    }
    return max_fd;
}

ConnectionManager::~ConnectionManager() {
    shutdownAllNow();
}

void ConnectionManager::shutdownNow(int connection_id) {
    std::unique_lock<std::mutex> lock(mutex);
    _shutdownNow(connection_id);
}

void ConnectionManager::_shutdownNow(int connection_id) const {
    try {
        Connection &conn = connections.at(connection_id);
        conn.getRequestQueue().shutdownConnectionNow();
    } catch (std::out_of_range e) {
        std::cerr << "No conneciton under id " << connection_id << std::endl;
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
    std::cout << std::endl << std::endl << "unregistered client " << id << std::endl << std::endl;
}

void ConnectionManager::collect(Connection& me) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.insert({me.getId(), me});
}

void ConnectionManager::readAll() {
    std::unique_lock<std::mutex> lock(mutex);

    for(auto it = connections.begin() ; it != connections.end() ; ++it) {
        Connection& conn = it->second;
        int conn_sock_fd = conn.getSock().getSockFd();
        if(FD_ISSET(conn_sock_fd, exception_fdset)) {
            //TODO handle socket exception
        }
        if(FD_ISSET(conn_sock_fd, listened_fdset)) {
            try {
                std::cout << "reading " << conn_sock_fd <<std::endl;
                conn.readReceivedData();
            }catch (ConnectionTerminationException e){
                _shutdownNow(conn.getId());
            }
        }
    }
}

void ConnectionManager::writeAll() {
    std::unique_lock<std::mutex> lock(mutex);
    for(auto it = connections.begin() ; it != connections.end() ; ++it) {
        Connection& conn = it->second;
        int conn_sock_fd = conn.getSock().getSockFd();
        if(FD_ISSET(conn_sock_fd, exception_fdset)) {
            //TODO handle socket exception
        }
        if(FD_ISSET(conn_sock_fd, write_fdset)) {
            networkPipe.readInt();
            conn.sendData();
        }
    }
}

ConnectionManager::ConnectionManager(Pipe &networkThreadPipe_)
    : connections(),
    networkPipe(networkThreadPipe_),
    mutex()
{}

void ConnectionManager::setFdSets(fd_set *listen, fd_set *write, fd_set *exception) {
    std::unique_lock<std::mutex> lock(mutex);
    listened_fdset = listen;
    write_fdset = write;
    exception_fdset = exception;
}

void ConnectionManager::addSender(Connection &c) {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << c.getId() << "wants to send\n";
    networkPipe.writeInt(c.getId());
}
