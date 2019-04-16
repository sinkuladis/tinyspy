#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#define BUFF_MSG_LEN 256

class Client {
  int socket;
  char buffer_in[BUFF_MSG_LEN];
  char buffer_out[BUFF_MSG_LEN];
  int id;

public:
  Client(int newSocket, int id = 0) : socket(newSocket), id(id){};
  ~Client();
  void mock_answer() const;
  int getSocket() { return socket; }
  bool operator==(const Client& other) const;
};

#endif // _CLIENT_HPP_
