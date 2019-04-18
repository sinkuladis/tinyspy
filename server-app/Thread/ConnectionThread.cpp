//
// Created by zmus on 16/04/19.
//

#include<pthread.h>
#include "./ConnectionThread.h"
#include "../Socket/Socket.h"
#include<sys/select.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/time.h>
#include <functional>
#include<algorithm>
#include <thread>

void ConnectionThread::run() {
    /*TODO nalezy odrzucic wywolanie, jesli thread juz dziala, z racji bezpieczenstwa
    moze byc kilka obiektow ConnectionThread, ale ta metoda moze byyc odpalona tylko raz, az skonczy sie ostatni thread - mutex trylock?*/
    run_mutex.lock();
    pthread_create(&thread_id, NULL, conn_routine, this);
    pthread_join(thread_id, NULL);
    run_mutex.unlock();
}

void* ConnectionThread::conn_routine(void* connectionThreadPtr) {
    ConnectionThread* ctptr = (ConnectionThread*)connectionThreadPtr; //TODO co jesli obiekt zostanie w tej chwili realokowany? -> musisz przekazac wskaznik na strukture, w ktorej polem bedzie referencja na obiekt
    ConnectionThread &connTrd = *ctptr;

    fd_set exception_fdset;
    fd_set& listened_fdset = std::ref(connTrd.listened_fds);

    int nfds, max_fd;

    struct timeval timeout;
    bool still_listening = true;
    while(still_listening) {
        timeout = {
            .tv_sec = 10,
            .tv_usec = 0
        };
        FD_ZERO(&connTrd.listened_fds); FD_ZERO(&exception_fdset);
        connTrd.connCollector.enter();
        max_fd = connTrd.initListenedFdSet();
        std::vector<int> conn_fds = connTrd.connCollector.getConnectionDescriptors();
        connTrd.connCollector.leave(); // czy jednak po dodaniu?
        exception_fdset = listened_fdset;

        nfds = select(max_fd+1, &listened_fdset, NULL, &exception_fdset, &timeout);
        if(nfds < 0){
            //TODO handle error
            continue;
        }
        else if (nfds == 0)
            continue;
        else{
            //first check exception set on both, then read set on then, then ex on connections and at the end read on connection fds
            if(FD_ISSET(connTrd.console_fd, &exception_fdset)) {
                //TODO handle console exceptions
                --nfds;
            }
            if(FD_ISSET(connTrd.console_fd, &listened_fdset)) {
                //TODO handle admin commands
                --nfds;
            }
            if(FD_ISSET(connTrd.listenSock.getSockFd(), &exception_fdset)){
                //TODO handle listenSock exceptions
                --nfds;
            }
            if(FD_ISSET(connTrd.listenSock.getSockFd(), &listened_fdset)) {
                connTrd.connCollector.enter();
                connTrd.connCollector.addConnection(connTrd.listenSock);
                --nfds;
                connTrd.connCollector.leave();
            }

            for (auto fd=conn_fds.begin() ; fd!=conn_fds.end() && nfds > 0 ; ++fd) {
                if (FD_ISSET(*fd, &exception_fdset)) {
                    //TODO handle connection exceptions
                    --nfds;
                }
                if(FD_ISSET(*fd, &listened_fdset)) {
                    connTrd.connCollector.readReceivedData(*fd);
                    --nfds;
                    connTrd.connCollector.sendData(*fd); //nie chcemy tego tutaj, bo polaczenie moze sie zakonczyc;
                    // to, co chcemy, to oddelegowanie przeczytanych danych do wyzszej warstwy, takze dzialajacej na conncollectorze
                    // warstwa taka wpierw by dane deserializowala(nie mamy tylko jeszcze zaimplementowanych takich mechanizmow)
                    // potem przesylalaby do warstwy interpretujacej polecenia
                    // na koniec ta powolalaby odpowiednie obiekty do pracy

                }
            }
            //a moze powinienem tutaj dopiero wyjsc z collectora? w koncu bede operowal na polaczeniach, ktore moga zostac przerwane przez ten

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
    listenSock.initialize();
    listenSock.bind(server_addr);
    listenSock.listen(max_pending_conns);
    listenSock.setNonblocking(); //TODO co ze statusem
}
