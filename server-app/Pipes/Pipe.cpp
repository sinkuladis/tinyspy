#include "Pipe.h"

#include <unistd.h>
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

int Pipe::read(char *buf, int nbytes) {
    int ret = RWOperation::read(output, buf, nbytes);
    return ret;
}

int Pipe::write(char *data, int nbytes) {
    int ret = RWOperation::write(input, data, nbytes);
    return ret;
}

int Pipe::writeInt(int id) {
    int nbytes = ::write(input, &id, sizeof(int));
    return nbytes;
}

int Pipe::readInt() {
    int conn_no=-1;
    int nbytes = ::read(output, &conn_no, sizeof(int));
    if (nbytes < 0)
        return -1 * abs(errno);
    return conn_no;
}