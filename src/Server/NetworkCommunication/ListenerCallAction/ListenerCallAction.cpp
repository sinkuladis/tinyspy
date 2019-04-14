#include "ListenerCallAction.hpp"
#include <iostream>

void connectClients(int mainSocket, struct sockaddr_in server) {
    while( true ) {
        printf( "Waiting for connection...\n" );
        struct sockaddr_in client = {};
        socklen_t len = sizeof( server );
        int clientSocket = accept(mainSocket,( struct sockaddr * ) & client, & len );
        if( clientSocket < 0 ) {
            perror( "accept() ERROR" );
            break;
        }
    }
    
}
