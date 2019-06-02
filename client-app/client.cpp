#include <arpa/inet.h>
#include <err.h>
#include <iostream>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include "Serialization/AuthMessage.h"
#include "Serialization/DataMessage.h"

struct addrinfo *get_sockaddr(const char *hostname, const char *port);

int open_connection(struct addrinfo *addr_list);

void sendMockData(int sockfd);

int readMockData(int sockfd);

int main(int argc, char *argv[]) {

    int numbytes;

    if (argc != 4) {
        printf("\n Usage: %s hostname port number_to_be_multiplied\n", argv[0]);
        return 1;
    }

    int j = 15;
    while(j--) {
        struct addrinfo *results = get_sockaddr(argv[1], argv[2]);
        int sockfd = open_connection(results);
        int number = atoi(argv[3]);
        for (int i = 0; i < number; ++i) {
            sendMockData(sockfd);
            numbytes = readMockData(sockfd);
            if (numbytes == -1) {
                perror("recv");
                exit(1);
            }
        }
    }
    return 0;
}

void sendMockData(int sockfd) {
    std::string output;
    DataMessage dataMsg;
    dataMsg.setMock();
    dataMsg.serialize(output);
    output.append("\0");

    int32_t msgLen = ntohl(output.length());
    int32_t msgType = ntohl(dataMsg.getType());
    write(sockfd, &msgType, sizeof(int32_t));
    write(sockfd, &msgLen, sizeof(int32_t));
    write(sockfd, output.data(), output.length());
}

int readMockData(int sockfd) {
    int msgLen = 0, msgType = 0;

    read(sockfd, &msgType, sizeof(int32_t));
    read(sockfd, &msgLen, sizeof(int32_t));
    msgLen = ntohl(msgLen);
    msgType = ntohl(msgType);

    std::vector<char> in_buffer;
    in_buffer.resize(msgLen);
    int readBytes = read(sockfd, in_buffer.data(), msgLen);

    std::string input(in_buffer.begin(), in_buffer.end());

    std::unique_ptr<TinyMessage> msg;

    if (msgType == DataType)
        msg = std::make_unique<DataMessage>(input);
    else if (msgType == AuthType)
        msg = std::make_unique<AuthMessage>(input);

    std::cout << msg->debugString() << std::endl;

    return readBytes;
}

int open_connection(struct addrinfo *addr_list) {

    struct addrinfo *p;
    int sockfd;

    for (p = addr_list; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd == -1)
            continue;

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) != -1)
            break;
    }

    freeaddrinfo(addr_list);

    if (p == nullptr)
        err(EXIT_FAILURE, "%s", "Unable to connect");
    else
        return sockfd;
}

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
