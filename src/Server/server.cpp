#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include <unistd.h>

#define MAX_MSG_LEN 4096
#define SERWER_PORT 8888
#define SERWER_IP "127.0.0.1"
#define MAX_CONNECTION 2

std::thread threads[MAX_CONNECTION];

void s_client(const int clientSocket) {
    char buffer[ MAX_MSG_LEN ] = { };
    memcpy(&buffer[0], "hi", strlen("hi"));

    if( recv( clientSocket, buffer, sizeof( buffer ), 0 ) <= 0 ) {
        perror( "recv() ERROR" );
        exit( 5 );
    }

    for (int i = 0 ; i < 5 ; i++){
        printf( "|Message from client|: %s \n", buffer );
        sleep(2);
    }

    strcpy( buffer, "blabla" );
    if( send( clientSocket, buffer, strlen( buffer ), 0 ) <= 0 ) {
        perror( "send() ERROR" );
        exit( 6 );
    }

    shutdown( clientSocket, SHUT_RDWR );
    std::cout << "To juz jest koniec" << std::endl;
    pthread_exit((void *)0);
}

int main() {
    int i = 0;
    struct sockaddr_in serwer = {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
    };

    if( inet_pton( AF_INET, SERWER_IP, & serwer.sin_addr ) <= 0 ) {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
   
    const int socket_ = socket( AF_INET, SOCK_STREAM, 0 );
    if( socket_ < 0 ) {
        perror( "socket() ERROR" );
        exit( 2 );
    }
   
    socklen_t len = sizeof( serwer );
    if( bind( socket_,( struct sockaddr * ) & serwer, len ) < 0 ) {
        perror( "bind() ERROR" );
        // zamkniecie socketu
        exit( 3 );
    }
   
    if( listen( socket_, MAX_CONNECTION ) < 0 ) {
        perror( "listen() ERROR" );
        exit( 4 );
    }
   
   
    while( true ) {
        printf( "Waiting for connection...\n" );
       
        struct sockaddr_in client = { };
       
        const int clientSocket = accept( socket_,( struct sockaddr * ) & client, & len );
        if( clientSocket < 0 ) {
            perror( "accept() ERROR" );
            continue;
        }

        new std::thread(s_client, clientSocket);
        i = ( i + 1 ) % MAX_CONNECTION;
    }
    shutdown( socket_, SHUT_RDWR );
}
//   gcc server.c -g -Wall -o server && ./server
