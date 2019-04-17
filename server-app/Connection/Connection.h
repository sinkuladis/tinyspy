//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTION_H
#define TINYSPY_CONNECTION_H


#include "../Socket/Socket.h"

class Connection {
protected:
    Socket sock;
public:
    virtual void readReceivedData();
    virtual void delegateWork();
    Socket getSock() { return sock;}
};


#endif //TINYSPY_CONNECTION_H
