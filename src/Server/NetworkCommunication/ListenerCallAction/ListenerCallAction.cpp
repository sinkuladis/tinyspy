#include "ListenerCallAction.hpp"
#include "../../Client/Client.hpp"
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <future>

#define MAX_MSG_LEN 4096

void addNewClient(int clientSocket, std::list<Client> &clients) {
  Client newClient = Client(clientSocket, clients.size());
  clients.push_back(newClient);
  newClient.mock_answer();
}


void connectClients(int mainSocket, struct sockaddr_in server, std::list<Client> &clients, std::future<void> futureObj) {
    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        struct sockaddr_in client = {};
        socklen_t len = sizeof( server );
        int clientSocket = accept(mainSocket,( struct sockaddr * ) & client, & len );
        if(clientSocket > 0) {
            addNewClient(clientSocket, clients);
            std::cout << "Dodano nowego klienta" << std::endl;
        }
    }
    std::cout << "Wylaczanie odbierania polaczen" << std::endl;
}
