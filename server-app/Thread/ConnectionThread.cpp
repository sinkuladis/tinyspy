//
// Created by zmus on 16/04/19.
//

#include <pthread.h>
#include "./ConnectionThread.h"
#include "../Socket/Socket.h"
#include "../Console/CommandCode.h"
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <functional>
#include <algorithm>
#include <thread>

void ConnectionThread::run() {
    run_mutex.lock();
    running = true;
    pthread_create(&thread_id, NULL, conn_routine, this);
}

void ConnectionThread::join() {
    pthread_join(thread_id, NULL);
    run_mutex.unlock();
}


void* ConnectionThread::conn_routine(void* connectionThreadPtr) {
    //to jest prawdziwe piękno pthreadów. static na konkretnym obiekcie
    ConnectionThread &connTrd = *((ConnectionThread*)connectionThreadPtr); //TODO co jesli obiekt zostanie w tej chwili realokowany? -> musisz przekazac wskaznik na strukture, w ktorej polem bedzie referencja na obiekt
    ConnectionCollector& connCollector = connTrd.connCollector;
    Pipe& executorPipe = connTrd.executorPipe;
    Pipe& consolePipe = connTrd.consolePipe;
    Socket& listenSock = connTrd.listenSock;
    fd_set& listened_fdset = std::ref(connTrd.listened_fdset);
    fd_set& exception_fdset = std::ref(connTrd.exception_fdset);

    int nfds, max_fd;
    std::vector<int> conn_fds;
    struct timeval timeout;
    while(connTrd.running) {
        timeout = {
            .tv_sec = 10,
            .tv_usec = 0
        };

        connCollector.enter();
        //std::cout<< " conn entered\n";

        max_fd = connTrd.initFdSets();
        conn_fds = connCollector.getConnectionDescriptors();

//        std::cout<< " conn left\n";
        connCollector.leave();

        nfds = select(max_fd+1, &listened_fdset, NULL, &exception_fdset, &timeout);
        std::cout<<"select returned "<<nfds<<std::endl;
        if(nfds < 0){
            //TODO handle error

        }
        else if (nfds == 0){

        }
        else{
            //first check exception set on both, then read set on then, then ex on connections and at the end read on connection fds
            if(FD_ISSET(consolePipe.getOutputFd(), &exception_fdset)) {
                //TODO handle console exceptions
                --nfds;
            }
            if(FD_ISSET(consolePipe.getOutputFd(), &listened_fdset)) {
                --nfds;
                int command = consolePipe.readInt();
                connTrd.runCommand(command);
                if(!connTrd.running)
                    break;
            }
            if(FD_ISSET(listenSock.getSockFd(), &exception_fdset)){
                //TODO handle listenSock exceptions
                --nfds;
            }
            if(FD_ISSET(listenSock.getSockFd(), &listened_fdset)) {
                connCollector.addConnection(listenSock);
                --nfds;
            }
            //std::cout<<"enter loop"<<std::endl;
            for (auto fd=conn_fds.begin() ; fd!=conn_fds.end() && nfds > 0 ; ++fd) {
                if (FD_ISSET(*fd, &exception_fdset)) {
                    //TODO handle connection exceptions
                    --nfds;
                }
                if(FD_ISSET(*fd, &listened_fdset)) {

                    int conn_id = *fd;
                    connCollector.enter();
  //                  std::cout<< " conn entered to read\n";
                    connCollector.readReceivedData(conn_id);
    //                std::cout<< " conn left reading\n";
                    executorPipe.writeInt(conn_id);
                    std::cout<<"exe thread was notified of"<< conn_id<<std::endl;
                    connCollector.leave();
                    --nfds;
                }
            }
        }
        //jesli administrator wyda polecenie odrzucenia danego klienta w innym watku, to nie mozemy konczyc polaczenia w trakcie jego obslugi, stad wychodzimy z moitora dopiero tutaj
        sleep(3); //bez tego connection thread jest za szybki i nie wpuszcza executora do conncollectora XD
    }
    std::cout<<"Connection thread exited"<<std::endl;
    return nullptr;
}

int ConnectionThread::initFdSets() {

    FD_ZERO(&listened_fdset);
    FD_ZERO(&exception_fdset);

    int max_fd = connCollector.getConnectionsFdSet(&listened_fdset);
    connCollector.getConnectionsFdSet(&exception_fdset);

    int listen_sock_fd = listenSock.getSockFd();
    int console_fd = consolePipe.getOutputFd();

    FD_SET(listen_sock_fd, &listened_fdset);
    FD_SET(console_fd, &listened_fdset);

    max_fd = std::max({max_fd, listen_sock_fd, console_fd});

    return max_fd;
}

void ConnectionThread::initListeningSocket(sockaddr_in server_addr) {
    listenSock.initialize();
    listenSock.bind(server_addr);
    listenSock.listen(max_pending_conns);
    listenSock.setNonblocking(); //TODO co ze statusem
}

