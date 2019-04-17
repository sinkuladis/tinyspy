//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTIONMANAGER_H
#define TINYSPY_CONNECTIONMANAGER_H

#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <unordered_map>
#include "Connection.h"

class ConnectionCollector{
private:
    std::unordered_map<int, Connection> connections;
public:
    ConnectionCollector()=default;
    ConnectionCollector(std::unordered_map<int, Connection> &nConnections) : connections(nConnections) {}

    int getConnectionsFdSet(fd_set*); // monitored method
    int[] getConnectionDescriptors();
    void readReceivedData(const Connection& conn);
    void readReceivedData(const int sock_fd);
    void addConnection(const int sock_fd);
};


#endif //TINYSPY_CONNECTIONMANAGER_H
