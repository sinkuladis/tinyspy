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
    std::string read(int nbytes=1024);
    int write(char*);
};

#endif // PIPE_H
