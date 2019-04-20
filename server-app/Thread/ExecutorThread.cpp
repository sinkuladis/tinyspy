//
// Created by zmus on 18/04/19.
//
#include "ExecutorThread.h"
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <algorithm>

//powinno dzialac w oddzielnym threadzie - mainExecutorThread

void ExecutorThread::listenOnPipe() {
    bool running=true;
    int n_ready_fd;
    int max_fd = std::max({connectionPipe.getOutputFd(), consolePipe.getOutputFd()});
    struct timeval timeout;
    while(running) {
        initFdSets();
        timeout = {
                .tv_sec = 2,
                .tv_usec = 0
        };
        n_ready_fd = select(max_fd+1, &listened_pipes, NULL, &exception_pipes, &timeout);
        if(n_ready_fd < 0) {
            //TODO handle select error
        }

        else if(n_ready_fd == 0)
            continue;

        if ( FD_ISSET(consolePipe.getOutputFd(), &exception_pipes) ) {
            //TODO handle console pipe exceptions
        }

        if ( FD_ISSET(consolePipe.getOutputFd(), &listened_pipes) ) {
            //TODO handle console pipe commands
        }

        if ( FD_ISSET(connectionPipe.getOutputFd(), &exception_pipes) ) {
            //TODO handle connection pipe exceptions
        }

        if ( FD_ISSET(connectionPipe.getOutputFd(), &listened_pipes) ) {
            pthread_t* new_thread = new pthread_t;
            pthread_create(new_thread, NULL, &ExecutorThread::handleConnection, this);
        }
    }
}

void ExecutorThread::initFdSets() {
    FD_ZERO(&listened_pipes);
    FD_ZERO(&exception_pipes);
    FD_SET(connectionPipe.getOutputFd(), &listened_pipes);
    FD_SET(consolePipe.getOutputFd(), &listened_pipes);
    FD_SET(connectionPipe.getOutputFd(), &exception_pipes);
    FD_SET(consolePipe.getOutputFd(), &exception_pipes);
}

void ExecutorThread::run() {
    mainThread = std::thread(&ExecutorThread::listenOnPipe, this); //assigns the state of the started thread to mainThread, it's NOT a copy assignment
}

void ExecutorThread::join() {
    mainThread.join();
}

/*
 * W tym miejscu bedziemy w finalnej wersji programu zapewne odbierac informacji o polaczeniu do obsluzenia
 * nastepnie bedziemy musieli zlecic deserializacje danych odebranych od polaczenia przekazanego przez pipeline od ConnectionThreada
 * skolejkowac zdeserializowane dane jako obiekty typu request prosto do kolejki requestow obiektu connection
 * zasygnalizowac jakiemus RequestHandlerowi poprzez pipeline gotowosc do dzialania i niech on juz sie tym dalej zajmuje
 *
 * trzeba bedzie jedynie dodac kilka pipeline'ow i przekazac je do kolejnych obiektow threadow przy tworzeniu tychze threadow - wiec
 * modulowosc mamy zapewniona
*/
 void* ExecutorThread::handleConnection(void* exe_trd_ptr) {
    ExecutorThread& exe = *((ExecutorThread*)exe_trd_ptr); //pthready sa piekne
    ConnectionCollector& connCollector = exe.connCollector;
    Pipe& connectionPipe = exe.connectionPipe;

     std::cout<<"Handle connection was called\n";
     int conn_id = connectionPipe.readInt();
     if(conn_id < 0) {
         std::cout << "nothing to read on the connectionPipe " << std::endl;
         pthread_exit(0);
     }

     std::cout<<"exe read"<<conn_id<<std::endl;
     connCollector.enter();
     std::cout<<"exe entered\n";

     Connection& conn = connCollector.at(conn_id);
     Request req = conn.getNextRequest();

     switch (req.getCode()){
         case ANSW:
             conn.mockAnswer();
             break;
         case TERM:
             connCollector.removeConnection(conn_id);
             break;
     }

     std::cout<<"exe left\n";
     connCollector.leave();
}
