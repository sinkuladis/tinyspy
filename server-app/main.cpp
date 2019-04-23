//
// Created by zmus on 16/04/19.
//
#include "Server.h"
#include "Pipes/Pipe.h"

int main(int argc, char**argv) {
    if(argc != 3) {
        printf("Usage: %s port max_connection\n",argv[0]);
        return 1;
    }
    int port = std::atoi(argv[1]);
    int max_connections = std::atoi(argv[2]);

    Server server(port, max_connections);
    server.start();
}