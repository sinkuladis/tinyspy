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

struct addrinfo *get_sockaddr(const char *hostname, const char *port) {

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

int open_connection(struct addrinfo *addr_list) {

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

int main(int argc, char *argv[]) {

  int numbytes;
  char sendBuff[MAX_MSG_SIZE];
  char recvBuff[MAX_MSG_SIZE];
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int rv;
  int sum;

  if (argc != 4) {
    printf("\n Usage: %s hostname port number_to_be_multiplied\n", argv[0]);
    return 1;
  }

  struct addrinfo *results = get_sockaddr(argv[1], argv[2]);
  int sockfd = open_connection(results);
int number = atoi(argv[3]);
for(int i = 0; i < number ; ++i) {
  memset(sendBuff, '0', sizeof(sendBuff));
  memset(recvBuff, '0', sizeof(recvBuff));

  snprintf(sendBuff, sizeof(sendBuff), "%d", number);
  write(sockfd, sendBuff, strlen(sendBuff));
/*
  if(number % 2 ==1){
    std::cout<<"written, spanko"<<std::endl;
    sleep(10);
  }
*/
  numbytes = recv(sockfd, recvBuff, sizeof(recvBuff) - 1, 0);
  if (numbytes == -1) {
    perror("recv");
    exit(1);
  }
/*
if(number % 2 ==0){
    std::cout<<"recvd, spanko"<<std::endl;
    sleep(10);
  }
*/
  recvBuff[numbytes] = '\0';
  std::cout << i+1 << '\t' << recvBuff << std::endl;
}
  return 0;
}
