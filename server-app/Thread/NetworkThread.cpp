//
// Created by zmus on 16/04/19.
//

#include <pthread.h>

#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#include <functional>
#include <algorithm>

#include "../Connection/executor_args.h"
#include "NetworkThread.h"
#include "../Socket/Socket.h"

void NetworkThread::run() {
    run_mutex.lock();
    running = true;
    pthread_create(&thread_id, NULL, conn_routine, this);
}

void NetworkThread::join() {
    pthread_join(thread_id, NULL);
    run_mutex.unlock();
}


void* NetworkThread::conn_routine(void* netThreadPtr) {
    NetworkThread &netThread = *((NetworkThread*)netThreadPtr);
    ConnectionManager& connMgr = netThread.connMgr;
    Pipe& consolePipe = netThread.consolePipe;
    Socket& listenSock = netThread.listenSock;
    fd_set& listened_fdset = std::ref(netThread.listened_fdset);
    fd_set& exception_fdset = std::ref(netThread.exception_fdset);

    int nfds, max_fd;
    struct timeval timeout;
    while(netThread.running) {
        timeout = {
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

        max_fd = netThread.initFdSets();

        nfds = select(max_fd + 1, &listened_fdset, NULL, &exception_fdset, &timeout);
        std::cout << "select returned " << nfds << std::endl;
        if (nfds < 0) {
            //TODO handle error
        } else {
            if (FD_ISSET(consolePipe.getOutputFd(), &exception_fdset)) {
                //TODO handle console exceptions
            }
            if (FD_ISSET(consolePipe.getOutputFd(), &listened_fdset)) {
                int command = consolePipe.readInt();
                netThread.runCommand(command);
                if (!netThread.running)
                    break;
            }
            if (FD_ISSET(listenSock.getSockFd(), &exception_fdset)) {
                //TODO handle listenSock exceptions
            }
            if (FD_ISSET(listenSock.getSockFd(), &listened_fdset))
                netThread.acceptNewConnection();

            connMgr.readAll(&listened_fdset, &exception_fdset);
        }
        sleep(3); //bez tego connection thread jest za szybki i nie wpuszcza executora do conncollectora XD
    }
    std::cout<<"Connection thread exited"<<std::endl;
    return nullptr;
}

void NetworkThread::acceptNewConnection() {
    Socket newSock = listenSock.accept();
    int connection_id = newSock.getSockFd();
    struct executor_args args = {
            .connMgr = connMgr,
            .sock = newSock
    };
    pthread_t thrd;
    pthread_create(&thrd, NULL, &Connection::executor_routine, &args);
    pthread_detach(thrd);

    std::cout << "Added client #" << connection_id << std::endl;
}

int NetworkThread::initFdSets() {
    int max_fd = connMgr.getConnectionsFdSet(&listened_fdset, &exception_fdset);

    int listen_sock_fd = listenSock.getSockFd();
    int console_fd = consolePipe.getOutputFd();
    FD_SET(listen_sock_fd, &listened_fdset);
    FD_SET(console_fd, &listened_fdset);
    FD_SET(listen_sock_fd, &exception_fdset);
    FD_SET(console_fd, &exception_fdset);

    max_fd = std::max({max_fd, listen_sock_fd, console_fd});
    return max_fd;
}

void NetworkThread::initListeningSocket(sockaddr_in server_addr) {
    listenSock.initialize();
    listenSock.bind(server_addr);
    listenSock.listen(max_pending_conns);
    listenSock.setNonblocking(); //TODO co ze statusem
}

