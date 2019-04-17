#include "ClientConnection/clientconnection.h"

int main(int argc, char *argv[]) {

  if (argc != 4) {
    printf("\n Usage: %s hostname port number_to_be_multiplied\n", argv[0]);
    return 1;
  }

  ClientConnection connection(argv[1], argv[2]);

  connection.clearBuffers();

  connection.writeSocket();
  connection.readSocket();
  std::cout << connection.getRecvBuff() << std::endl;

  return 0;
}
