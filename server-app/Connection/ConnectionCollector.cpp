//
// Created by zmus on 16/04/19.
//

#include <functional>
#include <sys/select.h>
#include "ConnectionCollector.h"

Connection& ConnectionCollector::addConnection(Socket& listenSock) {
    Socket& newSock = listenSock.accept(); //jesli bedzie potrzeba zapisac informacje o polaczeniu, bedzie mozna zrobic to tutaj i przekazac odpowiednia strukture adresu nowemu polaczeniu
    bool ok;
    Connection* newConnect = new Connection(newSock);
    ok = connections.insert( {newSock.getSockFd(), std::ref(*newConnect) } ).second;

    int i = 0;
    d_test.push_back(i);

    return std::ref(connections.at(newSock.getSockFd()));
    //if not ok socket was already in use though it shouldnt happen since its the os which assigns descriptors
}

Connection &ConnectionCollector::at(const int sock_fd) {
    return std::ref(connections.at(sock_fd));
}

void ConnectionCollector::readReceivedData(Connection &conn) {
    try {
        conn.readReceivedData();
    }catch (std::exception e) {
        connections.erase(conn.getSock().getSockFd());
    }

}

void ConnectionCollector::readReceivedData(const int sock_fd) {
    readReceivedData(std::ref(connections.at(sock_fd)));
}

int ConnectionCollector::getConnectionsFdSet(fd_set *fdsetptr) {
    FD_ZERO(fdsetptr);
    int max_fd = -1;
    for(auto& conn : connections){
        FD_SET(conn.first, fdsetptr);
        max_fd = conn.first > max_fd ? conn.first : max_fd;
    }
    return max_fd;
}

std::vector<int> ConnectionCollector::getConnectionDescriptors() {
    std::vector<int> fdVec;
    for(auto& conn : connections)
        fdVec.push_back(conn.first);
    return fdVec;
}
