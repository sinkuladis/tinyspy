//
// Created by zmus on 16/04/19.
//

#include <cstdlib>
#include <iterator>
#include <Serialization/AuthMessage.h>
#include "Connection.h"
#include "executor_args.h"
#include "../Exception/ConnectionTerminationException.h"
#include "ConnectionState.h"

Connection::Connection(Socket nSock, ConnectionManager& manager) : in_buffer(1024, 0), out_buffer(1024, 0), readbytesleft(0), readoffs(0), myManager(manager){
    sock = nSock;
    state = IDLE;
}

void Connection::readReceivedData() {
    if(readbytesleft == 0)
        switchReadState();

    int readbytes = 0;
    switch(state) {
        case RTYP:
            readbytes = sock.read(&mtype+readoffs, readbytesleft);
            break;
        case RSIZ:
            readbytes = sock.read(&msize+readoffs, readbytesleft);
            break;
        case RMES:
            readbytes = sock.read(in_buffer.data()+readoffs, readbytesleft);
    }

    if(readbytes == 0)
        throw ConnectionTerminationException();

    readoffs += readbytes;
    readbytesleft -= readbytes;

    if(readbytesleft == 0 and state==RMES)
        switchReadState();
}


void Connection::sendData() {
    int writebytes = 0;

    void* msg=outMessageQueue.get_message();
    int size=outMessageQueue.get_message_size();
    writebytes = sock.write(msg, outMessageQueue.get_message_size()-outMessageQueue.get_offset());
    outMessageQueue.push_offset(writebytes);

    if(writebytes == -1)
        throw ConnectionTerminationException();
    //else if(size!=writebytes)
    //    myManager.addSender(*this);
}

void Connection::switchReadState() {
    switch (state) {
        case IDLE:
            state = RTYP;
            readbytesleft = sizeof(int32_t);
            break;
        case RTYP:
            state = RSIZ;
            readbytesleft = sizeof(int32_t);
            break;
        case RSIZ:
            state = RMES;
            in_buffer.clear();
            readbytesleft = ntohl(msize);
            in_buffer.reserve(readbytesleft);
            break;
        case RMES:
            requestQueue.enqueue(DECYPHER);
            state = IDLE;
            readbytesleft = 0;
            break;
    }
    readoffs = 0;
}

void Connection::mockAnswer() {
    std::string mockAnswer("Howdy, client #" + std::to_string(getId()));

    std::string output;
    DataMessage dataMsg;
    dataMsg.setMock();
    dataMsg.setText(mockAnswer);
    dataMsg.serialize(output);
    output.append("\0");

    int32_t msgLen = ntohl(output.length());
    int32_t msgType = ntohl(dataMsg.getType());

    char *ptr = (char*)&msgType;
    std::string s ="";
    for(int i=0;i<4;++i, ++ptr)
        s+=*ptr;
    ptr = (char*)&msgLen;
    for(int i=0;i<4;++i, ++ptr)
        s+=*ptr;

    std::string msg=s+output;

    outMessageQueue.add_message(OutMessage(msg));
    myManager.addSender(*this);
}

void *Connection::executor_routine(void *executor_args_) {
    struct executor_args *args = static_cast<executor_args*>(executor_args_);
    ConnectionManager& connMgr = *(args->connMgr);
    Connection& conn = std::ref(*(args->newConn));
    free(executor_args_);

    while(conn.state != SHUT) {
        Request r = conn.requestQueue.getNext();
        conn.handleRequest(r);
    }

    delete &conn;
}

void Connection::handleRequest(Request request) {
    int reqcode = request.getCode();
    switch(reqcode) {
        case ANSW:
            mockAnswer();
            break;

        case TERM:
            terminate();
            break;

        case DECYPHER:
            requestQueue.enqueue(DESERIALIZE);
            break;

        case DESERIALIZE:
            deserialize();
            break;
    }
}

void Connection::terminate() {
    state = SHUT;
}

RequestQueue &Connection::getRequestQueue() {
    return requestQueue;
}

void Connection::deserialize() {
    std::string input(in_buffer.begin(), in_buffer.end());
    std::unique_ptr<TinyMessage> msg;
    if (mtype == DataType)
        msg = std::make_unique<DataMessage>(input);
    else if (mtype == AuthType)
        msg = std::make_unique<AuthMessage>(input);
    mtype = -1;
    msize = -1;
    requestQueue.enqueue(ANSW);
}

bool Connection::isReadyToSend() const {
    return outMessageQueue.not_empty();
}

Connection & Connection::startExecutor(executor_args *args) {
    pthread_t thrd;
    Connection *conn_ptr =  new Connection(args->sock, std::ref(*args->connMgr));
    args->newConn = conn_ptr;
    pthread_create(&thrd, NULL, &Connection::executor_routine, args);
    pthread_detach(thrd);
    return std::ref(*conn_ptr);
}

Socket Connection::getSock() { return sock;}

void Connection::printMessage() { std::cout<<"Client #"<<getId()<<" said "<< std::string(in_buffer.data()) <<std::endl; }

Connection::~Connection() {
    myManager.unregister(getSock().getSockFd());
    sock.shut();
}
