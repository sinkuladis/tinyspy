#ifndef _LISTENERCALLACTION_HPP_
#define _LISTENERCALLACTION_HPP_


#include <netinet/in.h>
#include "../../Client/Client.hpp"
#include <future>
#include <list>

void connectClients(int, struct sockaddr_in, std::list<Client>&, std::future<void> futureObj);

#endif  // _LISTENERCALLACTION_HPP_
