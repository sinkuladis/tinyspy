#include "Client.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

Client::~Client() {
    std::cout << "Zamykanie socketu: " << socket << std::endl;
    shutdown( socket, SHUT_RDWR );
}

void Client::mock_answer() const {
  std::string answer = "Hi, Client " + std::to_string(id);
  write(socket, answer.data(), sizeof(answer));
}

bool Client::operator==(const Client& other) const {
    return socket == other.socket && id == other.id;
}
