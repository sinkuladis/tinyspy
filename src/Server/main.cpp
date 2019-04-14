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
#define MAX_TRY 3
#define BASIC_SLEEP 2

std::thread threads[MAX_CONNECTION];
int mainSocket;

/*
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
*/

void shut(int status) {
    std::cout << "Closing socket " << mainSocket << std::endl;
    shutdown( mainSocket, SHUT_RDWR );
    exit (status);
}

int createMainSocket() {
    int currTry = 1;
    do {
        mainSocket = socket( AF_INET, SOCK_STREAM, 0 );
        if( mainSocket < 0 ) {
            perror( "ERROR while creating main socket:" + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && mainSocket < 0);

    if ( mainSocket < 0 ) {
        perror( "ERROR while creating main socket. Shutdown" );
        exit(1);
    }
    else
        std::cout << "Socket created: " << mainSocket << std::endl;
    return mainSocket;

}

struct sockaddr_in createServer() {
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons( SERWER_PORT )
    };
    return server;
}

void bindSocket(int socket, sockaddr_in &server) {
    socklen_t sockLen = sizeof( server );
    int currTry = 1;
    int bindStatus;

    do {
        bindStatus = bind( socket,( struct sockaddr * ) & server, sockLen );
        if( bindStatus < 0 ) {
            perror( "ERROR while binding socket: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && bindStatus < 0);

    if ( bindStatus < 0 )
        shut(1);
    else
        std::cout << "Socket binded." << std::endl;
}

void startListening(int socket, int maxConnection) {
    int currTry = 1;
    int listenStatus;
    do {
        listenStatus = listen( socket, maxConnection );
        if( listenStatus  < 0 ) {
            perror( "ERROR while listening: " + currTry++ );
            sleep(BASIC_SLEEP);
        }
    } while (currTry <= MAX_TRY && listenStatus < 0);

    if ( listenStatus < 0 )
        shut(2);
    else
        std::cout << "Listening." << std::endl;
}

int main() {
   // int i = 0;
    mainSocket = createMainSocket();
    struct sockaddr_in serwer = createServer();
    bindSocket(mainSocket, serwer);
    startListening(mainSocket, MAX_CONNECTION);

/*
    if( inet_pton( AF_INET, SERWER_IP, & serwer.sin_addr ) <= 0 ) {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
*/

    while( true ) {
        printf( "Waiting for connection...\n" );
       
        struct sockaddr_in client = {};
        socklen_t len = sizeof( serwer );
        int clientSocket = accept(mainSocket,( struct sockaddr * ) & client, & len );
        if( clientSocket < 0 ) {
            perror( "accept() ERROR" );
            break;
        }

        //new std::thread(s_client, clientSocket);
        //i = ( i + 1 ) % MAX_CONNECTION;
    }
    shut(0);
    return 0;
}
//   gcc server.c -g -Wall -o server && ./server
