//
// Created by zmus on 16/04/19.
//

#include<pthread.h>
#include "./ConnectionThread.h"
#include "../Socket/Socket.h"
#include<sys/select.h>
#include<sys/types.h>s
#include<unistd.h>
#include<sys/time.h>
#include <functional>
#include<algorithm>
#include <thread>

typedef void * (ConnectionThread::*NonStaticMethodPtr)(void *);
typedef void * (*FuncPtr)(void*);

void ConnectionThread::run() {
    /*TODO nalezy odrzucic wywolanie, jesli thread juz dziala, z racji bezpieczenstwa
    moze byc kilka obiektow ConnectionThread, ale ta metoda moze byyc odpalona tylko raz, az skonczy sie ostatni thread - mutex trylock?*/
    pthread_create(&thread_id, NULL, conn_routine, this);
}

void* ConnectionThread::conn_routine(void* connectionThreadPtr) {
    ConnectionThread* ctptr = (ConnectionThread*)connectionThreadPtr; //TODO co jesli obiekt zostanie w tej chwili realokowany? -> musisz przekazac wskaznik na strukture, w ktorej polem bedzie referencja na obiekt
    ConnectionThread &connTrd = *ctptr;

    fd_set exception_fds;
    fd_set& listened_fds = std::ref(connTrd.listened_fds);
    //int listenSock = connTrd.listenSock.getSockFd();

    int nfds, max_fd;

    struct timeval timeout;
    bool still_listening = true;
    while(still_listening) {
        timeout = {
            .tv_sec = 10,
            .tv_usec = 0
        };
        FD_ZERO(&connTrd.listened_fds); FD_ZERO(&exception_fds);
        max_fd = connTrd.initListenedFdSet();
        exception_fds = listened_fds;

        nfds = select(max_fd+1, &listened_fds, NULL, &exception_fds, &timeout);
        if(nfds < 0){
            //TODO handle error
        }
        else if (nfds == 0)
            continue;
        else{
            //first check exception set on both, then read set on then, then ex on connections and at the end read on connection fds
            if(FD_ISSET(connTrd.console_fd, &exception_fds)) {
                //TODO handle console exceptions
            }
            if(FD_ISSET(connTrd.console_fd, &listened_fds)) {
                //TODO handle admin commands
            }
            if(FD_ISSET(connTrd.listenSock.getSockFd(), &exception_fds)){
                //TODO handle listenSock exceptions
            }
            if(FD_ISSET(connTrd.listenSock.getSockFd(), &listened_fds)) {
                int new_sockfd = connTrd.listenSock.accept(); //jesli bedziemy zbierac w obiektach polaczen info o adresie, mozemy utworzyc sockaddr_in i przekazac jego adres
                connTrd.connCollector.addConnection(new_sockfd);
            }


        }

    }
    return nullptr;
}

int ConnectionThread::initListenedFdSet() {
    int max_fd = connCollector.getConnectionsFdSet(&listened_fds);
    int listen_sock_fd = listenSock.getSockFd();
    FD_SET(listen_sock_fd, &listened_fds);
    FD_SET(console_fd, &listened_fds);
    return std::max({max_fd, listen_sock_fd, console_fd});
}

void ConnectionThread::initListeningSocket(sockaddr_in server_addr) {
    listenSock.bind(server_addr);
    listenSock.listen(max_pending_conns);
    listenSock.setNonblocking(); //TODO co ze statusem
}
