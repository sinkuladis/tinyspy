#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

constexpr const int MAX_MSG_LEN = 4096;
constexpr const char SERWER_IP[] = "127.0.0.1";
constexpr const int PORT = 8888;
constexpr const int MAX_CONNECTION = 2;
constexpr const int MAX_TRY = 3;
constexpr const int BASIC_SLEEP = 2;

class Socket {
public:
  Socket(int port = PORT, int maxConnection = MAX_CONNECTION);
  ~Socket();

  int createSocket();
  void createAddr();
  int bindSocket();
  int startListening();
  int setNonblocking();
  void shut();

private:
  int m_socket;
  int m_port;
  int m_maxConnection;
  struct sockaddr_in m_sockaddr;
};

#endif // SOCKET_H
