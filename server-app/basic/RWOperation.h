//
// Created by zmus on 29/04/19.
//

#ifndef TINYSPY_RWOPERATION_H
#define TINYSPY_RWOPERATION_H


#include <netinet/in.h>

class RWOperation {
public:
    int read(int file_descriptor, char *buf, int nbytes = 1024);
    int write(int file_descriptor, char *output, int nbytes);
};


#endif //TINYSPY_RWOPERATION_H
