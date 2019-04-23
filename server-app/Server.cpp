//
// Created by zmus on 16/04/19.
//

#include "Server.h"

void Server::start() {
    networkThread.initListeningSocket(server_addr); //nie jestem fanem tego rozwiazania, ale 2. wyjscie to trzymanie takiego samego server_addr w threadzie, a 3. to przekazywanie w 500 miejsc i cast na i potem z powtorem z void*
    networkThread.run();

    consoleHandler.handleConsole();

    networkThread.join();
}

int Server::getListeningPort() { return ntohs(server_addr.sin_port); }

Server::Server(int listening_port, int max_pend_conn)
        : consoleToNetworkPipe(Pipe()),
        //connectionToSerializerPipe(Pipe()),
        //serializerToExecutorPipe(Pipe()),
          //connectionToExecutorPipe(Pipe()),
          //consoleToExecutorPipe(Pipe()),
          connCollector(),
          consoleHandler(std::ref(consoleToNetworkPipe), std::ref(connCollector)),
          networkThread( std::ref(consoleToNetworkPipe) , max_pend_conn, std::ref(connCollector))
          //,executorMainThread(std::ref(connectionToExecutorPipe), std::ref(consoleToExecutorPipe), std::ref(connCollector))
{
    server_addr = {
            .sin_family=AF_INET,
            .sin_port=htons(listening_port)
    };
}

