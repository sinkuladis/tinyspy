//
// Created by zmus on 19/04/19.
//

#ifndef TINYSPY_CONNECTIONTERMINATIONEXCEPTION_H
#define TINYSPY_CONNECTIONTERMINATIONEXCEPTION_H

#include "ConnectionException.h"

class ConnectionTerminationException : public ConnectionException {
public:
    ConnectionTerminationException(int n_conn_id = 0, std::string nMessage="Connection terminated due to remote app exit") : ConnectionException(nMessage, n_conn_id) {}
private:
    int n_conn_id;
};


#endif //TINYSPY_CONNECTIONTERMINATIONEXCEPTION_H
