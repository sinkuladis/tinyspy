//
// Created by zmus on 16/04/19.
//

#include "Server.h"

void Server::start() {


    connectionThread.initListeningSocket(server_addr); //nie jestem fanem tego rozwiazania, ale 2. wyjscie to trzymanie takiego samego server_addr w threadzie, a 3. to przekazywanie w 500 miejsc i cast na i potem z powtorem z void*
    connectionThread.run();
    executorMainThread.run();

    executorMainThread.join();
    connectionThread.join();
}