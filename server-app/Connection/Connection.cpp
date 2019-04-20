//
// Created by zmus on 16/04/19.
//

#include <cstdlib>
#include <iterator>
#include "Connection.h"

Connection::Connection(Socket nSock) {
    sock = nSock;
}

void Connection::readReceivedData() {
    int readbytes = sock.read(in_buffer, 1);
    int req = readbytes == 0 ? TERM : ANSW; // deserializacja na miare makeshiftu XD
    requestQueue.push_back(Request( req ));
}

void Connection::writeDataToSend(char*data) {
    sock.write(data, 0);
}

Request Connection::getNextRequest() {
    Request req = requestQueue.front();
    requestQueue.pop_front();
    return req;
}

void Connection::mockAnswer() {
    std::string mockAnswer("Howdy, client #");
    mockAnswer.append( std::to_string(getId()) );
    char* data = const_cast<char *>(mockAnswer.data());
    sock.write(data,mockAnswer.length());
}

