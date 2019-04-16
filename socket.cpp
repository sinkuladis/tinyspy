#include "socket.h"

Socket::Socket(int port, int maxConnection)
    : m_port(port), m_maxConnection(maxConnection) {}

Socket::~Socket() {
  // TODO: check socket value after shutdown
  if (m_socket != 0)
    shut();
}

int Socket::createSocket() {
  int currTry = 1;
  do {
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
      perror("ERROR while creating main socket:" + currTry++);
      sleep(BASIC_SLEEP);
    }
  } while (currTry <= MAX_TRY && m_socket < 0);

  if (m_socket < 0) {
    perror("ERROR while creating main socket. Shutdown");
    // TODO: proper error handling
    return -1;
  } else
    std::cout << "Socket created: " << m_socket << std::endl;
  return 0;
}

void Socket::createAddr() {
  m_sockaddr = {.sin_family = AF_INET, .sin_port = htons(m_port)};
}

int Socket::bindSocket() {
  socklen_t sockLen = sizeof(m_sockaddr);
  int currTry = 1;
  int bindStatus;

  do {
    bindStatus = bind(m_socket, (struct sockaddr *)&m_sockaddr, sockLen);
    if (bindStatus < 0) {
      perror("ERROR while binding socket: " + currTry++);
      sleep(BASIC_SLEEP);
    }
  } while (currTry <= MAX_TRY && bindStatus < 0);

  if (bindStatus < 0) {
    shut();
    return -1;
  } else
    std::cout << "Socket binded." << std::endl;
  return 0;
}

int Socket::startListening() {

  int currTry = 1;
  int listenStatus;
  do {
    listenStatus = listen(m_socket, m_maxConnection);
    if (listenStatus < 0) {
      perror("ERROR while listening: " + currTry++);
      sleep(BASIC_SLEEP);
    }
  } while (currTry <= MAX_TRY && listenStatus < 0);

  if (listenStatus < 0) {
    shut();
    return -1;
  } else
    std::cout << "Listening." << std::endl;
  return 0;
}

int Socket::setNonblocking() {
  int flags;
  if (-1 == (flags = fcntl(m_socket, F_GETFL, 0)))
    flags = 0;
  return fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
}

void Socket::shut() {
  std::cout << "Zamykanie socketu: " << m_socket << std::endl;
  shutdown(m_socket, SHUT_RDWR);
}
