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
#include "NetworkCommunication/ListenerCallAction/ListenerCallAction.hpp"
#include "socketCreator/socketCreator.hpp"

#define MAX_MSG_LEN 4096
#define SERWER_PORT 8888
#define SERWER_IP "127.0.0.1"
#define MAX_CONNECTION 2
#define MAX_TRY 3
#define BASIC_SLEEP 2

std::thread threads[MAX_CONNECTION];

int main() {
    int mainSocket;
    createMainSocket(mainSocket);
    struct sockaddr_in serwer = createServer();
    bindSocket(mainSocket, serwer);
    startListening(mainSocket, MAX_CONNECTION);
    connectClients(mainSocket, serwer);

    shut(0, mainSocket);
    return 0;
}
