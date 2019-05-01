//
// Created by zmus on 16/04/19.
//

#include <cstdlib>
#include <iterator>
#include "Connection.h"
#include "executor_args.h"
#include "../Exception/ConnectionTerminationException.h"
#include "ConnectionState.h"

Connection::Connection(Socket nSock) {
    sock = nSock;
    state = ONGOING;
}

void Connection::readReceivedData() {
    int readbytes = sock.read(in_buffer, 1);
    int req = readbytes == 0 ? TERM : ANSW; // deserializacja na miare makeshiftu XD
    requestQueue.enqueue(Request( req ));
}

void Connection::writeDataToSend(char*data) {
    sock.write(data, 0);
}

void Connection::mockAnswer() {
    std::string mockAnswer("Howdy, client #");
    mockAnswer.append( std::to_string(getId()) );
    char* data = const_cast<char *>(mockAnswer.data());
    sock.write(data,mockAnswer.length());
}

void *Connection::executor_routine(void *args_) {
    struct executor_args *args = static_cast<executor_args*>(args_);
    ConnectionManager& connMgr = *(args->connMgr);
    Connection conn(args->sock);
    free(args);

    connMgr.collect(conn);

    while(conn.state == ONGOING) {
        //w tym momencie polecenia z konsolki dotyczące stanu połączenia będą obsługiwane jako requesty i wszystko zostaje ułatwione 500x
        Request r = conn.requestQueue.getNext();
        conn.handleRequest(r);
    }

    connMgr.unregister(conn.getId());
}

void Connection::handleRequest(Request request) {
    int reqcode = request.getCode();
    switch(reqcode){
        case ANSW:
            mockAnswer();
            break;
        case TERM:
            terminate();
            break;
        default:
            //InvalidRequestException?
            break;
    }
}

void Connection::terminate() {
    state = SHUTDOWN;
}

RequestQueue &Connection::getRequestQueue() {
    return requestQueue;
}

