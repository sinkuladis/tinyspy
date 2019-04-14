#include <netinet/in.h>

void shut(int, int);
int createMainSocket(int &);
struct sockaddr_in createServer();
void bindSocket(int, sockaddr_in &);
void startListening(int, int);
