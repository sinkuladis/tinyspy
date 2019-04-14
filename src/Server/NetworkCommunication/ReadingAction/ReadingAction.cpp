#include "ReadingAction.hpp"
#include <sys/types.h>
#include <sys/socket.h>

int readMSG(int socket,char *data, int dataSize)
{

    int size;
    recv(socket,&size,4,0);
    int offset=0;
    if(size>dataSize) size=dataSize;	 
    while(offset!=size)
    {
	offset+=recv(socket,data+offset,size-offset,0);
    } 
    return size;
}

