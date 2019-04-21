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
    std::mutex mutex;
    std::unordered_map<int,Connection&> connections;
public:
    ConnectionCollector()=default;
    ~ConnectionCollector();

    int getConnectionsFdSet(fd_set*); // monitored method
    std::vector<int> getConnectionDescriptors();
    void readReceivedData(Connection& conn);
    void readReceivedData(const int sock_fd);
    void sendData(Connection& conn);
    void sendData(const int sock_fd);
    Connection& addConnection(Socket&);
    Connection& at(int conn_id);
    void removeConnection(int);
    void enter();
    void leave();
};


#endif //TINYSPY_CONNECTIONMANAGER_H
