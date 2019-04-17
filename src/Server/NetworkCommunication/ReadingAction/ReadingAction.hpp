#ifndef _READINGACTION_HPP_
#define _READINGACTION_HPP_

#include"../../Client/Client.hpp"
#include<list>
#include <sys/types.h>
struct selectArgs {
    std::list<Client>& clients;
    fd_set* cli_set_ptr;
    int fd;
};
void* prepSelect(void* select_args);
void readClientMessage(Client& client);
void selectForReading(fd_set* cli_fd_set_ptr, std::list<Client> &clients, int fd);

#endif // READINGACTION_HPP
