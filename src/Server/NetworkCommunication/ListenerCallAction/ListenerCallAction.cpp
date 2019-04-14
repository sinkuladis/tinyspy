#include "ListenerCallAction.hpp"
#include "../../Client/Client.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>

#define MAX_MSG_LEN 4096

void s_client(const int clientSocket) {
    char buffer[ MAX_MSG_LEN ] = {};
    memcpy(&buffer[0], "hi", strlen("hi"));

    if( recv( clientSocket, buffer, sizeof( buffer ), 0 ) <= 0 ) {
        perror( "recv() ERROR" );
        exit( 5 );
    }

    for (int i = 0 ; i < 5 ; i++){
        printf( "|Message from client|: %s \n", buffer );
        sleep(2);
    }

    std::strcpy( buffer, "blabla" );
    if( send( clientSocket, buffer, strlen( buffer ), 0 ) <= 0 ) {
        perror( "send() ERROR" );
        exit( 6 );
    }

    shutdown( clientSocket, SHUT_RDWR );
    std::cout << "To juz jest koniec" << std::endl;
    pthread_exit((void *)0);
}


void addNewClient(int clientSocket) {
    Client client = Client(clientSocket);
    client.sayHi();
}


void connectClients(int mainSocket, struct sockaddr_in server) {
    while( true ) {
        printf( "Waiting for connection...\n" );
        struct sockaddr_in client = {};
        socklen_t len = sizeof( server );
        int clientSocket = accept(mainSocket,( struct sockaddr * ) & client, & len );
        addNewClient(clientSocket);
    }
}
