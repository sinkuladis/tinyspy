#include <netinet/in.h>

void shut(int, int);
int createMainSocket();
struct sockaddr_in createServer(int);
void bindSocket(int, sockaddr_in &);
void startListening(int, int);
