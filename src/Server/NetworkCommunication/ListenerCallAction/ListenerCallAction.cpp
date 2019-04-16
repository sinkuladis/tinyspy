#include "ListenerCallAction.hpp"
#include "../../Client/Client.hpp"
#include "../../socketCreator/socketCreator.hpp"
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <future>

#define MAX_MSG_LEN 4096

void addNewClient(int clientSocket, std::list<Client> &clients) {
  Client *newClient = new Client(clientSocket, clients.size());
  clients.push_back( std::ref( *newClient ) );
  //newClient.mock_answer();
}


void *connectClients(void *servData) {
    struct connectArgs *tmpServData = (struct connectArgs*)servData;

    std::list<Client> &clients = std::ref(tmpServData->clients);
    int serverSocket = createMainSocket();
    struct sockaddr_in server = getServerStruct(tmpServData->port);
    bindSocket(serverSocket, server);
    startListening(serverSocket, tmpServData->maxConnection);
    setNonblocking(serverSocket);

    while (true) {
        struct sockaddr_in client = {};
        socklen_t len = sizeof( server );
        int clientSocket = accept(serverSocket,( struct sockaddr * ) & client, & len );
        if(clientSocket > 0) {
            addNewClient(clientSocket, clients);
            std::cout << "Dodano nowego klienta" << std::endl;
        }
    }
    std::cout << "Wylaczanie odbierania polaczen" << std::endl;
}
