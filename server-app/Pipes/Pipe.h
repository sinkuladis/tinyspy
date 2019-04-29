#ifndef PIPE_H
#define PIPE_H

#include <utility>
#include <string>
#include "../basic/RWOperation.h"

class Pipe : protected RWOperation
{
private:
    int input;
    int output;
public:
    Pipe();
    int getInputFd();
    int getOutputFd();
    int writeInt(int fd);
    int write(char *data, int nbytes);
    int read(char *buf, int nbytes);
    int readInt();
};

#endif // PIPE_H
