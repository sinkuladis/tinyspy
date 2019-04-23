//
// Created by zmus on 19/04/19.
//

#ifndef TINYSPY_CONNECTIONEXCEPTION_H
#define TINYSPY_CONNECTIONEXCEPTION_H


#include <bits/exception.h>
#include <string>
class ConnectionException : public std::exception {
private:
    std::string message;
    int connection_id;
public:
    ConnectionException(std::string nMessage, int n_connection_id) : message(nMessage), connection_id(n_connection_id) {}
    std::string getMessage() {return message;}
    int getConnectionId() {return connection_id; }
};


#endif //TINYSPY_CONNECTIONEXCEPTION_H
