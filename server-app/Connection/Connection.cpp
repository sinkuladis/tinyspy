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

Connection::Connection(Socket nSock) : in_buffer(1024, 0), out_buffer(1024, 0), readbytesleft(0), readyToSend(false) {
    sock = nSock;
    state = IDLE;
}

void Connection::readReceivedData() {
    if(readbytesleft == 0)
        switchState();

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
}

void Connection::switchState() {
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
            in_buffer.resize(readbytesleft);
            break;
        case RMES:
            requestQueue.enqueue(DECYPHER);
            state = IDLE;
            readbytesleft = 0;
            break;
    }
    readoffs = 0;
}

void Connection::writeDataToSend(char *data) {
    sock.write(data, 0);
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
    sock.write(&msgType);
    sock.write(&msgLen);
    sock.write(output.data(), output.length());
}

void *Connection::executor_routine(void *args_) {
    struct executor_args *args = static_cast<executor_args*>(args_);
    ConnectionManager& connMgr = *(args->connMgr);
    Connection conn(args->sock);
    free(args);

    connMgr.collect(conn);

    while(conn.state != SHUT) {
        //w tym momencie polecenia z konsolki dotyczące stanu połączenia będą obsługiwane jako requesty i wszystko zostaje ułatwione 500x
        Request r = conn.requestQueue.getNext();
        conn.handleRequest(r);
    }

    connMgr.unregister(conn.getId());
}

void Connection::handleRequest(Request request) {
    int reqcode = request.getCode();
    switch(reqcode) {
        case ANSW:
            readyToSend = true;
            break;

        case TERM:
            terminate();
            break;

        case DECYPHER:
            //na razie nie ma co robić
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
    return readyToSend;
}
