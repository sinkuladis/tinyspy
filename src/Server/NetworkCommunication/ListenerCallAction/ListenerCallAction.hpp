#include <netinet/in.h>
#include "../../Client/Client.hpp"
#include <list>

void connectClients(int, struct sockaddr_in, std::list<Client>&);
