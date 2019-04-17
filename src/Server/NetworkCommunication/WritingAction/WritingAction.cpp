#include "WritingAction.hpp"
#include <sys/types.h>
#include <sys/socket.h>


void writeMSG(int socket, const char *data, int dataSize)
{
    int32_t len = dataSize;
    send(socket,&len,4,0);
    int offset=0;
    while(offset!=dataSize)
    {
	offset+=send(socket,data+offset,dataSize-offset,0);
    }
    

}

