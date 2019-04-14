#include "Client.hpp"
#include <string>
#include <unistd.h>

void Client::mock_answer() const {
  std::string answer = "Hi, Client " + std::to_string(id);
  write(socket, answer.data(), sizeof(answer));
}
