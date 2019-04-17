#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

// TODO: zamykanie gnizd przy rozlączeniu klienta

class ClientConnection {
public:
  ClientConnection(std::string hostname, std::string port);

  struct addrinfo *get_sockaddr(const char *hostname, const char *port);
  int open_connection(struct addrinfo *addr_list);

  const char *getRecvBuff() const;
  const char *getSendBuff() const;

  size_t getRecvBuffSize() const;
  size_t getSendBuffSize() const;

  void writeSocket();
  void readSocket();

  void clearBuffers();

private:
  int numbytes;
  char sendBuff[MAX_MSG_SIZE];
  char recvBuff[MAX_MSG_SIZE];
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int rv;
  int sum;

  struct addrinfo *results;
  int sockfd;
};

#endif // CLIENTCONNECTION_H
