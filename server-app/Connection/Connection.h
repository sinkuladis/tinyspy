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
#include "../OutMessageQueue/OutMessageQueue.h"
#include "../OutMessage/OutMessage.h"
#include "executor_args.h"

class ConnectionManager;
struct executor_args;

class Connection {
protected:
    Socket sock;
    int32_t msize;
    int32_t mtype;

    std::vector<char> in_buffer;
    std::vector<char> out_buffer;
    RequestQueue requestQueue;
    OutMessageQueue outMessageQueue;

    int state;
    int readbytesleft;
    int readoffs;

    ConnectionManager& myManager;

    void handleRequest(Request request);
    void mockAnswer();
    void terminate();
    static void* executor_routine(void *executor_args_);

public:
    Connection(Socket nSock, ConnectionManager& manager);
    ~Connection();

    void readReceivedData();
    void sendData();
    void printMessage();

    Socket getSock();
    RequestQueue &getRequestQueue() ;

    int getId() { return sock.getSockFd(); }

    static Connection & startExecutor(executor_args *args);

    void switchReadState();

    bool isReadyToSend() const;

    void deserialize();
};


#endif //TINYSPY_CONNECTION_H
