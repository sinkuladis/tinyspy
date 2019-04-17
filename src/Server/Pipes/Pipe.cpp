#include "Pipe.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdexcept>
#include <fcntl.h>

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



