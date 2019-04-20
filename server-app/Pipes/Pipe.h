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
    int writeInt(int fd);
    int write(char*);
    std::string read(int nbytes);
    int readInt();
};

#endif // PIPE_H
