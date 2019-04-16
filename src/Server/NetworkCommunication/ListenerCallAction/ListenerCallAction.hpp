#ifndef _LISTENERCALLACTION_HPP_
#define _LISTENERCALLACTION_HPP_


#include <netinet/in.h>
#include "../../Client/Client.hpp"
#include <future>
#include <list>

void *connectClients(void *);
struct connectArgs {
    std::list<Client> &clients;
    int port;
    int maxConnection;    
};
#endif  // _LISTENERCALLACTION_HPP_
