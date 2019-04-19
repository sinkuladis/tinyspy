#ifndef PIPE_H
#define PIPE_H

#include <utility>
#include <string>

class Pipe
{
private:
    int input;
    int output;
public:
    Pipe();
    int getInputFd();
    int getOutputFd();
    int write(int fd);
    int write(char*);
    std::string read(int nbytes);
    int readConnNo();
};

#endif // PIPE_H
