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
#include "Pipes/Pipe.h"

class Connection;
struct executor_args;

class ConnectionManager{
private:
    std::mutex mutex;
    std::unordered_map<int,Connection&> connections;
    void _shutdownNow(int connection_id) const;
    Pipe& networkPipe;

    fd_set* listened_fdset;
    fd_set* write_fdset;
    fd_set* exception_fdset;

public:
    ConnectionManager(Pipe &networkThreadPipe_);

    void setFdSets(fd_set* listened_fdset, fd_set* write_fdset, fd_set* exception_fdset);

    ~ConnectionManager();

    int getConnectionsFdSet();

    void shutdownNow(int);
    void shutdownAllNow();

    void collect(Connection&);
    void unregister(int);

    void addSender(Connection& c);

    void readAll();
    void writeAll();
};


#endif //TINYSPY_CONNECTIONMANAGER_H
