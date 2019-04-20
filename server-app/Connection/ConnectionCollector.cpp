//
// Created by zmus on 16/04/19.
//

#include <functional>
#include <sys/select.h>
#include "ConnectionCollector.h"

Connection& ConnectionCollector::addConnection(Socket& listenSock) {
    Socket newSock = listenSock.accept(); //jesli bedzie potrzeba zapisac informacje o polaczeniu, bedzie mozna zrobic to tutaj i przekazac odpowiednia strukture adresu nowemu polaczeniu
    Connection* newConnect = new Connection(newSock);
    bool ok = connections.insert( {newSock.getSockFd(), *newConnect } ).second; //if not ok socket was already in use though it shouldnt happen since its the os which assigns descriptors
    if(ok) std::cout<<"Added client #"<<newConnect->getId()<<std::endl;
    return std::ref(connections.at(newSock.getSockFd()));
}

void ConnectionCollector::readReceivedData(Connection &conn) {
        conn.readReceivedData();
}

void ConnectionCollector::readReceivedData(const int sock_fd) {
        connections.at(sock_fd).readReceivedData();
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

void ConnectionCollector::sendData(Connection &conn) {
    std::string mockAnswer("Howdy, client #");
    mockAnswer.append( std::to_string(conn.getId()) );
    char* data = const_cast<char *>(mockAnswer.data());
    conn.writeDataToSend(data);
}

void ConnectionCollector::sendData(const int sock_fd) {
    sendData(connections.at(sock_fd));
}

void ConnectionCollector::removeConnection(int connection_id) {
    Connection& conn = connections.at(connection_id);
    connections.erase(connection_id); //niektore kontenery zalatwiaja destrukcje obiektow, ktore sie z nich wyrzuca, ale nie ten
    delete &conn;
}
