#ifndef PIPE_H
#define PIPE_H

#include <utility>

class Pipe
{
private:
    int input;
    int output;
public:
    Pipe();
    int getInputFd();
    int getOutputFd();
};

#endif // PIPE_H
