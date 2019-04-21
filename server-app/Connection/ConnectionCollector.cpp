//
// Created by zmus on 16/04/19.
//

#include <functional>
#include <sys/select.h>
#include <unordered_map>
#include "ConnectionCollector.h"

Connection& ConnectionCollector::addConnection(Socket& listenSock) {
    Socket newSock = listenSock.accept(); //jesli bedzie potrzeba zapisac informacje o polaczeniu, bedzie mozna zrobic to tutaj i przekazac odpowiednia strukture adresu nowemu polaczeniu
    Connection* newConnect = new Connection(newSock);
    connections.insert( {newConnect->getId(), *newConnect} );
    std::cout<<"Added client #"<<newConnect->getId()<<std::endl;
    return *newConnect;
}

void ConnectionCollector::readReceivedData(Connection &conn) {
        conn.readReceivedData();
}

void ConnectionCollector::readReceivedData(const int sock_fd) {
        at(sock_fd).readReceivedData();
}

int ConnectionCollector::getConnectionsFdSet(fd_set *fdsetptr) {
    FD_ZERO(fdsetptr);
    int max_fd = -1;
    for(auto& conn : connections){
        FD_SET(conn.second.getId(), fdsetptr);
        max_fd = conn.second.getId() > max_fd ? conn.second.getId() : max_fd;
    }
    return max_fd;
}

std::vector<int> ConnectionCollector::getConnectionDescriptors() {
    std::vector<int> fdVec;
    for(auto& conn : connections)
        fdVec.push_back(conn.second.getId());
    return fdVec;
}

void ConnectionCollector::sendData(Connection &conn) {
    std::string mockAnswer("Howdy, client #");
    mockAnswer.append( std::to_string(conn.getId()) );
    char* data = const_cast<char *>(mockAnswer.data());
    conn.writeDataToSend(data);
}

void ConnectionCollector::sendData(const int sock_fd) {
    sendData(at(sock_fd));
}

void ConnectionCollector::removeConnection(int connection_id) {
    Connection& conn = connections.at(connection_id);
    connections.erase(connection_id); //niektore kontenery zalatwiaja destrukcje obiektow, ktore sie z nich wyrzuca, ale nie unoderedmap
    delete &conn;
}

ConnectionCollector::~ConnectionCollector() {
    mutex.lock();
    for(auto c = connections.begin(); c != connections.end() ; ) {
        delete &c->second;
        c = connections.erase(connections.begin());
    }
    mutex.unlock();
}

Connection& ConnectionCollector::at(int conn_id) {
    return std::ref(connections.at(conn_id));
}

void ConnectionCollector::enter() { mutex.lock(); }

void ConnectionCollector::leave() { mutex.unlock(); }
