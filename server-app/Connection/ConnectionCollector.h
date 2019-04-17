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

class ConnectionCollector{
private:
    std::unordered_map<int, Connection> connections;
    std::mutex mutex;
public:
    ConnectionCollector()=default;

    int getConnectionsFdSet(fd_set*); // monitored method
    std::vector<int> getConnectionDescriptors();
    void readReceivedData(Connection& conn);
    void readReceivedData(const int sock_fd);
    Connection& at(const int sock_fd);
    Connection& addConnection(Socket&);
    void enter() { mutex.lock(); }
    void leave() { mutex.unlock(); }
};


#endif //TINYSPY_CONNECTIONMANAGER_H
