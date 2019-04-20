#include "Pipe.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdexcept>
#include <fcntl.h>
#include <cstring>
#include <iostream>

Pipe::Pipe()
{
    int fd[2];
    int result = pipe(fd);
    if(result == -1)
    {
        throw new std::runtime_error("cannot create a pipe");
    }
    if (fcntl(fd[0], F_SETFL, O_NONBLOCK) == -1) {
 	throw new std::runtime_error("Call to fcntl failed.");
    }
    input=fd[1];
    output=fd[0];
}

int Pipe::getInputFd()
{
    return input;
}

int Pipe::getOutputFd()
{
    return output;
}
//FIXME
std::string Pipe::read(int nbytes) {
    char* buf = (char*) calloc(nbytes, 1);

    int ret = ::read(output, buf, nbytes);

    std::string deb(buf);
    free(buf);
    return deb;
}
//FIXME
int Pipe::write(char*  data) {
    int ret = ::write(input, data, sizeof(data));
    return ret;
}

int Pipe::writeInt(int id) {
    int nbytes = ::write(input, &fd, sizeof(int));
    return nbytes;
}

int Pipe::readInt() {
    int conn_no=-1;
    int nbytes = ::read(output, &conn_no, sizeof(int));
    if(nbytes < 0){

    }
    return conn_no;
}