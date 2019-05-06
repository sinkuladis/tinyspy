//
// Created by zmus on 29/04/19.
//

#include <functional>
#include <fcntl.h>
#include <unistd.h>
#include<iostream>
#include "RWOperation.h"

int RWOperation::read(int file_descriptor, char *buf, int nbytes) {
    int offs=0, read_bytes=0;
    while(nbytes > 0) {
        read_bytes = ::read(file_descriptor, buf + offs, nbytes);

        if(read_bytes == 0)
            break;

        offs+=read_bytes;
        nbytes-=read_bytes;
    }
    return offs;
}

int RWOperation::write(int file_descriptor, char *output, int nbytes) {
    int written_bytes=0, offs=0;
    while(nbytes > 0) {
        written_bytes = ::write(file_descriptor, output + offs, nbytes);

        offs+=written_bytes;
        nbytes-=written_bytes;
    }
    return offs;
}