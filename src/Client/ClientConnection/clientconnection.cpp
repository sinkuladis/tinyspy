#include "clientconnection.h"

ClientConnection::ClientConnection(std::string hostname, std::string port) {
  results = get_sockaddr(hostname.data(), port.data());
  sockfd = open_connection(results);
}

const char *ClientConnection::getRecvBuff() const { return recvBuff; }

const char *ClientConnection::getSendBuff() const { return sendBuff; }

size_t ClientConnection::getRecvBuffSize() const { return sizeof(recvBuff); }

size_t ClientConnection::getSendBuffSize() const { return sizeof(sendBuff); }

void ClientConnection::writeSocket() {
  write(sockfd, sendBuff, strlen(sendBuff));
}

void ClientConnection::readSocket() {
  numbytes = recv(sockfd, recvBuff, sizeof(recvBuff) - 1, 0);
  if (numbytes == -1) {
    perror("recv");
    close(sockfd);
    // TODO: proper error handling
    exit(1);
  }

  recvBuff[numbytes] = '\0';
  //  std::cout << recvBuff << std::endl;
}

void ClientConnection::clearBuffers() {
  memset(sendBuff, '0', sizeof(sendBuff));
  memset(recvBuff, '0', sizeof(recvBuff));
}

struct addrinfo *ClientConnection::get_sockaddr(const char *hostname,
                                                const char *port) {

  struct addrinfo hints;
  struct addrinfo *results;
  int rv;

  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  rv = getaddrinfo(hostname, port, &hints, &results);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }

  return results;
}

int ClientConnection::open_connection(struct addrinfo *addr_list) {

  struct addrinfo *p;
  int sockfd;

  for (p = addr_list; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if (sockfd == -1)
      continue;

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) != -1)
      break;
  }

  freeaddrinfo(addr_list);

  if (p == NULL)
    err(EXIT_FAILURE, "%s", "Unable to connect");
  else
    return sockfd;
}
