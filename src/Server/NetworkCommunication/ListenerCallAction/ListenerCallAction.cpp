#include "ListenerCallAction.hpp"
#include "../../Client/Client.hpp"
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>

#define MAX_MSG_LEN 4096

void addNewClient(int clientSocket, std::list<Client> &clients) {
  Client newClient = Client(clientSocket, clients.size());
  clients.push_back(newClient);
  newClient.mock_answer();
}

void connectClients(int mainSocket, struct sockaddr_in server,
                    std::list<Client> &clients) {
  while (true) {
    printf("Waiting for connection...\n");
    struct sockaddr_in client = {};
    socklen_t len = sizeof(server);
    int clientSocket = accept(mainSocket, (struct sockaddr *)&client, &len);
    addNewClient(clientSocket, clients);
  }
}
