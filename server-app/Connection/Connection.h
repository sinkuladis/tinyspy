//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTION_H
#define TINYSPY_CONNECTION_H


#include <iostream>
#include <list>
#include <vector>
#include "Serialization/DataMessage.h"
#include "../Socket/Socket.h"
#include "../Request/Request.h"
#include "../Request/RequestQueue.h"

class Connection {
protected:
    Socket sock;
    int32_t msize;
    int32_t mtype;

    std::vector<char> in_buffer;
    std::vector<char> out_buffer;
    RequestQueue requestQueue;
    int state;
    int readbytesleft;
    int readoffs;
    int writebytesleft;
    int writeoffs;

    bool readyToSend;

    void handleRequest(Request request);
    void mockAnswer();
    void terminate();

public:
    Connection(Socket nSock);
    ~Connection() { sock.shut(); }

    void readReceivedData();
    void writeDataToSend(char*);
    void printMessage() { std::cout<<"Client #"<<getId()<<" said "<< in_buffer.data()<<std::endl; }

    Socket getSock() { return sock;}
    RequestQueue &getRequestQueue() ;

    //testowa metoda, tymczasowo id polaczenia to zwiazany z jego gniazdem file descriptor, unikalny dla polaczenia w trakcie jego dzialania
    int getId() { return sock.getSockFd(); }
    static void* executor_routine(void*conn_sock);

    void switchState();

    bool isReadyToSend() const;

    void setBytesLeft();

    void readtype();

    void deserialize();
};


#endif //TINYSPY_CONNECTION_H
