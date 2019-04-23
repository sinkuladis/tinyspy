//
// Created by zmus on 20/04/19.
//

#ifndef TINYSPY_REQUEST_H
#define TINYSPY_REQUEST_H

#include "RequestCode.h"

class Request {
private:
    const int req_code;
    //const int connection_id;
public:
    Request(int n_req_code) : req_code(n_req_code) {}
    int getCode() { return req_code; }
};


#endif //TINYSPY_REQUEST_H
