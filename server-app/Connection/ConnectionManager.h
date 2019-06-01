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

class Connection;
struct executor_args;

class ConnectionManager{
private:
    std::mutex mutex;
    std::unordered_map<int,Connection&> connections;
    void _shutdownNow(int connection_id) const;

public:
    ConnectionManager();
  
    ~ConnectionManager();

    int getConnectionsFdSet(fd_set *listen, fd_set *send, fd_set *exc);

    void shutdownNow(int);
    void shutdownAllNow();

    void collect(Connection&);
    void unregister(int);

    void readAll(fd_set* listen, fd_set* exc);
    void writeAll(fd_set* write, fd_set* exc);
};


#endif //TINYSPY_CONNECTIONMANAGER_H
