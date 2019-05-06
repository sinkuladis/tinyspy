//
// Created by zmus on 16/04/19.
//

#include "Server.h"

void Server::start() {
    //FIXME tego ma tu nie być, kiedy skończymy ListetningSocket
    networkThread.initListeningSocket(server_addr);
    networkThread.run();

    consoleHandler.handleConsole();

    networkThread.join();
}

int Server::getListeningPort() { return ntohs(server_addr.sin_port); }

Server::Server(int listening_port, int max_pend_conn)
        : consoleToNetworkPipe(Pipe()),
          connMgr(),
          consoleHandler(std::ref(consoleToNetworkPipe), std::ref(connMgr)),
          networkThread( std::ref(consoleToNetworkPipe) , max_pend_conn, std::ref(connMgr))
{
    server_addr = {
            .sin_family=AF_INET,
            .sin_port=htons(listening_port)
    };
}

