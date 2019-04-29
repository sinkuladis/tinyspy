//
// Created by zmus on 16/04/19.
//

#include <pthread.h>
#include "NetworkThread.h"
#include "../Socket/Socket.h"
#include "../Console/CommandCode.h"
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <functional>
#include <algorithm>
#include <thread>
#include "../Exception/ConnectionTerminationException.h"

void NetworkThread::run() {
    run_mutex.lock();
    running = true;
    pthread_create(&thread_id, NULL, conn_routine, this);
}

void NetworkThread::join() {
    pthread_join(thread_id, NULL);
    run_mutex.unlock();
}


void* NetworkThread::conn_routine(void* connectionThreadPtr) {
    //to jest prawdziwe piękno pthreadów. static do wykonania na konkretnym obiekcie
    NetworkThread &connTrd = *((NetworkThread*)connectionThreadPtr); //TODO co jesli obiekt zostanie w tej chwili realokowany? -> musisz przekazac wskaznik na strukture, w ktorej polem bedzie referencja na obiekt
    ConnectionManager& connCollector = connTrd.connCollector;
    Pipe& consolePipe = connTrd.consolePipe;
    Socket& listenSock = connTrd.listenSock;
    fd_set& listened_fdset = std::ref(connTrd.listened_fdset);
    fd_set& exception_fdset = std::ref(connTrd.exception_fdset);

    int nfds, max_fd;
    std::vector<int> conn_fds;
    struct timeval timeout;
    while(connTrd.running) {
        timeout = { //buduje na podstawie timeouta, ktory zwroci mi listeningsocket - czyli czas, ktory minie do ponownej inicjalizacji
            .tv_sec = 10,
            .tv_usec = 0
        };
    //FIXME tutaj bedziemy jeszcze musieli sprawdzać stan obiektu klasy ListeningSocket.
    // ListeningSocket powinien mieć kilka stanów, wg dra Gawkowskiego minimum 3:
    // 1) czeka na accepcie i próbuje zaakcpetować jakieś połączenie
    // 2) zaakceptował połączenie -> (mój domysł) dołącza je do connection managera i przełącza się w stan 1)
    // 3) niegotowy - tu zlecamy ponowienie proby przygotowania;
    // możemy zlecać je zawsze w tym miejscu i w środku metody ListeningSocketa robić bind od nowa - tylko wtedy, kiedy jest to potrzebne
    // Tylko raz w tym miejscu wywołujemy taką inicjalizującję ListaningSocketa. Jeśli się nie uda, to taki obiekt nie przechodzi w stan 1)
    // i w efekcie przy następnym wywołaniu metody inicjalizującej go, próba inicjalizacji zostanie ponowiona.
    // Taka metoda powinna zwracać tutaj informację o tym, po jakim czasie będziemy chcieli ponowić proóbę inicjalizację.
    // Poniższy select powinien czekać właśnie tyle czasu. Select może jednak zostac przerwany poprzez komendy. Stąd należałoby w
    // środku inicjalizacji zobaczyć ile tego czasu minęło i zwrócić tę wartość tutaj.
    // struct timeval na Linuxie jest modyfikowany przez selecta, więc możemy po prostu przekazać tamtej metodzie zmienną timeout,
    // by sprawdziła czy jesteśmy już w stanie ponowić próbie inicjalizacji; jeśli przystąpimy i nie wyjdzie - zwracamy nowy, ustalony czas.
    // Jeśli wyjdzie - zwracamy nieskończony timeout
    // Jeśli socket jest nienastawiony, ale timeout nie jest zerowy, to wtedy zwracamy timeout, który ma nam pozostać;
    // Problem z idealnością tego rozwiązania jest taki, że wtedy czekamy troszeczkę więcej, niz faktycznie chcemy, ale nie będzie to razej u nas problemem.
    // Gdybyśmy chcieli, moglibyśmy zapisywać czasy wywołania inicjalizacji i kalkulować faktycznie ile mamy jeszcze poczekać do inicjalizacji, ale PO CO


        connCollector.enter();  //std::cout<< " conn entered\n";
        max_fd = connTrd.initFdSets();

        nfds = select(max_fd+1, &listened_fdset, NULL, &exception_fdset, &timeout);
        //leave here?
        std::cout<<"select returned "<<nfds<<std::endl;
        if(nfds < 0){
            //TODO handle error
        }
        else if (nfds == 0){

        }
        else{
            if(FD_ISSET(consolePipe.getOutputFd(), &exception_fdset)) {
                //TODO handle console exceptions
                --nfds;
            }
            if(FD_ISSET(consolePipe.getOutputFd(), &listened_fdset)) {
                --nfds;
                int command = consolePipe.readInt();
                connTrd.runCommand(command);
                if(!connTrd.running) { //seems cheap, might delete it later
                    connCollector.leave();
                    break;
                }
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
            //enter here?
            //FIXME ta pętla potrzebuje być wykonana w monitorze ConnectionManager, co wpłynie na to, że nie wywołamy niczego na nieaktywnych połączeniach
            conn_fds = connCollector.getConnectionDescriptors();
            for (auto fd=conn_fds.begin() ; fd!=conn_fds.end() && nfds > 0 ; ++fd) {
                if (FD_ISSET(*fd, &exception_fdset)) {
                    //TODO handle connection exceptions
                    --nfds;
                }
                if(FD_ISSET(*fd, &listened_fdset)) {
                    int conn_id = *fd;
                    try {
                        connCollector.getConnection(conn_id).readReceivedData();
                        connCollector.getNetworkPipe(conn_id).writeInt(conn_id);
                        std::cout<<"exe thread was notified of"<< conn_id<<std::endl;
                    }catch (ConnectionTerminationException e){
                        connCollector.shutdownConnection(conn_id);
                    }

                    --nfds;
                }
            }
        }
        connCollector.leave();
        sleep(3); //bez tego connection thread jest za szybki i nie wpuszcza executora do conncollectora XD
    }
    std::cout<<"Connection thread exited"<<std::endl;
    return nullptr;
}

//FIXME - niech connection collecor robi to wszystko, a nie tylko modyfikuje fdsety i zwraca max_fd
//   wtedy nie trzeba też będzie 2x wywoływać tej samej metody getConnectionFdSet
int NetworkThread::initFdSets() {

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

void NetworkThread::initListeningSocket(sockaddr_in server_addr) {
    listenSock.initialize();
    listenSock.bind(server_addr);
    listenSock.listen(max_pending_conns);
    listenSock.setNonblocking(); //TODO co ze statusem
}

