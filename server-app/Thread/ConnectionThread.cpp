//
// Created by zmus on 16/04/19.
//

#include <pthread.h>
#include "./ConnectionThread.h"
#include "../Socket/Socket.h"
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <functional>
#include <algorithm>
#include <thread>

void ConnectionThread::run() {
    run_mutex.lock();
    pthread_create(&thread_id, NULL, conn_routine, this);
}

void ConnectionThread::join() {
    pthread_join(thread_id, NULL);
    run_mutex.unlock();
}


void* ConnectionThread::conn_routine(void* connectionThreadPtr) {
    ConnectionThread* ctptr = (ConnectionThread*)connectionThreadPtr; //TODO co jesli obiekt zostanie w tej chwili realokowany? -> musisz przekazac wskaznik na strukture, w ktorej polem bedzie referencja na obiekt
    ConnectionThread &connTrd = *ctptr;

    fd_set exception_fdset;
    fd_set& listened_fdset = std::ref(connTrd.listened_fds);

    int nfds, max_fd;
    std::vector<int> conn_fds;
    struct timeval timeout;
    bool still_listening = true;
    while(still_listening) {
        timeout = {
            .tv_sec = 2,
            .tv_usec = 0
        };

        connTrd.connCollector.enter();
        max_fd = connTrd.initListenedFdSet();
        conn_fds = connTrd.connCollector.getConnectionDescriptors();
        FD_ZERO(&exception_fdset);
        exception_fdset = listened_fdset;

        nfds = select(max_fd+1, &listened_fdset, NULL, &exception_fdset, &timeout);
        if(nfds < 0){
            //TODO handle error

        }
        else if (nfds == 0){

        }
        else{
            //first check exception set on both, then read set on then, then ex on connections and at the end read on connection fds
            if(FD_ISSET(connTrd.consolePipe.getOutputFd(), &exception_fdset)) {
                //TODO handle console exceptions
                --nfds;
            }
            if(FD_ISSET(connTrd.consolePipe.getOutputFd(), &listened_fdset)) {
                //TODO handle admin commands
                --nfds;
            }
            if(FD_ISSET(connTrd.listenSock.getSockFd(), &exception_fdset)){
                //TODO handle listenSock exceptions
                --nfds;
            }
            if(FD_ISSET(connTrd.listenSock.getSockFd(), &listened_fdset)) {
                connTrd.connCollector.addConnection(connTrd.listenSock);
                --nfds;
            }

            for (auto fd=conn_fds.begin() ; fd!=conn_fds.end() && nfds > 0 ; ++fd) {
                if (FD_ISSET(*fd, &exception_fdset)) {
                    //TODO handle connection exceptions
                    --nfds;
                }
                if(FD_ISSET(*fd, &listened_fdset)) {
                    connTrd.connCollector.readReceivedData(*fd);
                    connTrd.executorPipe.write(*fd);
                    --nfds;
                    //connTrd.connCollector.sendData(*fd);
                    //nie chcemy tego tutaj, bo polaczenie moze sie zakonczyc;
                    // to, co chcemy, to oddelegowanie przeczytanych danych do wyzszej warstwy, takze dzialajacej na conncollectorze
                    // warstwa taka wpierw by dane deserializowala(nie mamy tylko jeszcze zaimplementowanych takich mechanizmow)
                    // potem przesylalaby do warstwy interpretujacej polecenia
                    // na koniec ta powolalaby odpowiednie obiekty do pracy
                    //zarzadzanie polaczeniami tez jest w jej kwestii; tym bardziej odpowiedzi klientom

                }
            }
        }
        connTrd.connCollector.leave();//jesli administrator wyda polecenie odrzucenia danego klienta w innym watku, to nie mozemy konczyc polaczenia w trakcie jego obslugi, stad wychodzimy z moitora dopiero tutaj
    }
    return nullptr;
}

int ConnectionThread::initListenedFdSet() {

    FD_ZERO(&listened_fds);
    int max_fd = connCollector.getConnectionsFdSet(&listened_fds);
    int listen_sock_fd = listenSock.getSockFd();
    int console_fd = consolePipe.getOutputFd();
    FD_SET(listen_sock_fd, &listened_fds);
    FD_SET(console_fd, &listened_fds);

    max_fd = std::max({max_fd, listen_sock_fd, console_fd});

    return max_fd;
}

void ConnectionThread::initListeningSocket(sockaddr_in server_addr) {
    listenSock.initialize();
    listenSock.bind(server_addr);
    listenSock.listen(max_pending_conns);
    listenSock.setNonblocking(); //TODO co ze statusem
}