#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#define BUFF_MSG_LEN 256

class Client{
    int socket;
    char buffer_in[ BUFF_MSG_LEN ];
    char buffer_out[ BUFF_MSG_LEN ];

    public:
    Client(int newSocket): socket(newSocket) {};
};

#endif  // _CLIENT_HPP_
