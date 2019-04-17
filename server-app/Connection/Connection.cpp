//
// Created by zmus on 16/04/19.
//

#include <cstdlib>
#include <iterator>
#include "Connection.h"

Connection::Connection(Socket nSock) : sock(nSock) {}

void Connection::readReceivedData() {
    int readbytes = sock.read(in_buffer);
    if(readbytes == 0)
        throw std::exception();
}
