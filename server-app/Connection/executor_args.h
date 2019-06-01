//
// Created by zmus on 30/04/19.
//

#ifndef TINYSPY_EXECUTOR_ARGS_H
#define TINYSPY_EXECUTOR_ARGS_H

#include "ConnectionManager.h"
#include "Connection.h"

class ConnectionManager;
class Connection;

struct executor_args {
    ConnectionManager* connMgr;
    Socket sock;
    Connection* newConn;
};

#endif //TINYSPY_EXECUTOR_ARGS_H
