#include "ReadingAction.hpp"
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include<unistd.h>
#include<iostream>
#include<errno.h>
#include<stdlib.h>

void serveClients(fd_set* cli_fd_set_ptr, int nclients, std::list<Client> &clients)
{
    for(auto c = clients.begin() ; c != clients.end() && nclients > 0 ; ++c) {
	int cliSock = c->getSocket();
	//std::cout<<"Checking "<<cliSock<<std::endl;
	if( FD_ISSET(cliSock, cli_fd_set_ptr) ) {
	     void* buf=calloc(1024,1);
	    //std::cout <<"Attempt at reading client "<<client.getSocket()<< std::endl;
	    int ret = read(cliSock, buf, sizeof(buf));
	    std::cout << "Klient " << cliSock << " napisal znakow: " << ret << std::endl;
	    free(buf);
	    if(ret == 0) { // END OF STREAM - POLACZENIE ZAMKNIETE - NALEZY WYRZUCIC KLIENTA;
		Client& client = std::ref(*c);		
		clients.remove(client); //UWAGA - SOCKET JEST OTWARTY, JESLI KLIENT GO SAM NIE ZAMYKA
		//delete &client;
		FD_CLR(cliSock, cli_fd_set_ptr);
		std::cout<<"Usunieto 'polaczenie' z klientem na sockecie: " << cliSock << std::endl;
	    }
	    else{
		c->mock_answer();
	    	std::cout <<"Odpowiedziano klientowi: "<< cliSock << std::endl;
	    }
	    --nclients;
	}
    }
}

void selectForReading(fd_set* cli_fd_set_ptr, std::list<Client> &clients, int fd){
    int nready, max_fd;
    struct timeval timeout;
    timeout.tv_usec = 0;
    char buf;
    while(true){
	if(read(fd,&buf,1)>0) break;
	timeout.tv_sec = 10;
	max_fd=0;	
	FD_ZERO(cli_fd_set_ptr);	
	for (auto c = clients.begin() ; c != clients.end() ; ++c){
	    int cliSock = c->getSocket();
 	    if( cliSock > 0){
		std::cout <<"Do fdsetu dodajemy: "<< cliSock << std::endl;
		FD_SET(cliSock, cli_fd_set_ptr);
		max_fd = cliSock > max_fd ? cliSock : max_fd;
	    }
	}
	nready=select(max_fd+1, cli_fd_set_ptr, NULL, NULL, &timeout);
	std::cout <<"Select returned "<< nready << std::endl;
	if(nready < 0 ){
	    std::cout <<"Select exited with "<< errno << std::endl;
	    exit(-1);
	}
	else serveClients(cli_fd_set_ptr, nready, std::ref(clients));
    }
    std::cout<<"Select ended "<<std::endl;
}

void* prepSelect(void* select_args) {
    struct selectArgs* args = (struct selectArgs*) select_args;
    selectForReading(args->cli_set_ptr, std::ref(args->clients), args->fd);
}
