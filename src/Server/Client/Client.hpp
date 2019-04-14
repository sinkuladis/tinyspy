#include <iostream>
#define BUFF_MSG_LEN 256

class Client{
    int socket;
    char buffer_in[ BUFF_MSG_LEN ];
    char buffer_out[ BUFF_MSG_LEN ];

    public:
    Client(int newSocket): socket(newSocket) {};
    void sayHi() {
        std::cout << "Hi" << std::endl;
    }
};
