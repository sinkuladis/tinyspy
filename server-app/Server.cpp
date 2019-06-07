//
// Created by zmus on 16/04/19.
//

#include "Server.h"

void Server::start() {

    networkThread.run();
    consoleHandler.handleConsole();
    networkThread.join();
}


Server::Server(int listening_port, int max_pend_conn)
        : consoleToNetworkPipe(Pipe()),
          managerToNetworkPipe(Pipe()),
          connMgr(managerToNetworkPipe),
          consoleHandler(std::ref(consoleToNetworkPipe), std::ref(connMgr)),
          networkThread( std::ref(managerToNetworkPipe), std::ref(consoleToNetworkPipe) , max_pend_conn, std::ref(connMgr),listening_port)
{

}

