//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTION_H
#define TINYSPY_CONNECTION_H


#include <iostream>
#include "../Socket/Socket.h"

class Connection {
protected:
    Socket sock;
    char in_buffer[1024];
    char out_buffer[1024];
    //std::list<Task> taskQueue;
public:
    Connection(Socket nSock);
    ~Connection() { sock.shut(); }

    void readReceivedData();
    void writeDataToSend();
    void printMessage() {std::cout<<"Client "<<sock.getSockFd()<<" said "<< in_buffer<<std::endl;}

    Socket getSock() { return sock;}
};


#endif //TINYSPY_CONNECTION_H
