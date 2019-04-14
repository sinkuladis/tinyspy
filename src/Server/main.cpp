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
#include <list>
#include <functional>
#include <future>
#include "NetworkCommunication/ListenerCallAction/ListenerCallAction.hpp"
#include "socketCreator/socketCreator.hpp"
#include "ConsoleHandler/ConsoleHandler.hpp"

#define MAX_MSG_LEN 4096
#define SERWER_IP "127.0.0.1"
#define THREADS 2
#define MAX_TRY 3
#define BASIC_SLEEP 2

std::thread threads[THREADS];

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s port max_connection\n",argv[0]);
        return 1;
    }
    std::promise<void> exitSignal;
    std::future<void> futureObj = exitSignal.get_future();
    std::list<Client> clients;
    int port = std::atoi(argv[1]);
    int maxConnection = std::atoi(argv[2]);
    int mainSocket = createMainSocket();
    struct sockaddr_in serwer = createServer(port);
    bindSocket(mainSocket, serwer);
    startListening(mainSocket, maxConnection);
    setNonblocking(mainSocket);


    std::thread connectionThread(connectClients, mainSocket, serwer, std::ref(clients), std::move(futureObj));
    std::thread consoleThread(handleConsole, std::ref(exitSignal));
    connectionThread.join();
    consoleThread.join();

    shut(mainSocket);
    while(clients.size() > 0) {
        shut(clients.front().getSocket());
        clients.pop_front();
    }
    return 0;
}
