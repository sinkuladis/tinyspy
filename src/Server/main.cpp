#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <list>
#include <functional>
#include <future>
#include <vector>
#include "NetworkCommunication/ListenerCallAction/ListenerCallAction.hpp"
#include "NetworkCommunication/ReadingAction/ReadingAction.hpp"
#include "socketCreator/socketCreator.hpp"
#include "ConsoleHandler/ConsoleHandler.hpp"
#include "Pipes/Pipe.h"

#define MAX_MSG_LEN 4096
#define SERWER_IP "127.0.0.1"
#define MAX_TRY 3
#define BASIC_SLEEP 2
#define NUM_THREADS 2

pthread_t vecOfThreads[NUM_THREADS];

void *connectionThread(void *id) {
    while(true)
        std::cout << "A";
}

void *consoleThread(void *id) {
    while(true)
        std::cout << "B";
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s port max_connection\n",argv[0]);
        return 1;
    }
    Pipe p;
    struct serverData servData = {
        .port = std::atoi(argv[1]),
        .maxConnection = std::atoi(argv[2])
    };
    std::list<Client> clients;
    fd_set cli_fdset;
    
    struct connectArgs conn_args = {
	.clients = std::ref(clients),
        .port = std::atoi(argv[1]),
        .maxConnection = std::atoi(argv[2]),
	.fd = p.getOutputFd()
    };

    struct selectArgs select_args = {
	.clients = std::ref(clients),
	.cli_set_ptr = &cli_fdset,
	.fd = p.getOutputFd()
    };
    pthread_create((&vecOfThreads[0]), NULL, connectClients, (void *)&conn_args );
    pthread_create((&vecOfThreads[1]), NULL, prepSelect, (void *)&select_args );
    handleConsole(p.getInputFd());
	
    if ( pthread_cancel(vecOfThreads[0]) == 0 )
        std::cout << "Watek zamkniety pomyslnie" << std::endl;
   if ( pthread_cancel(vecOfThreads[1]) == 0 )
        std::cout << "Watek zamkniety pomyslnie" << std::endl;

/*    consoleThread.join();
    closeThreads();
    connectionThread.join();

    shut(mainSocket);
    while(clients.size() > 0) {
        shut(clients.front().getSocket());
        clients.pop_front();
    }*/
    return 0;
}
