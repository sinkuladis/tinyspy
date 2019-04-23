//
// Created by zmus on 18/04/19.
//
#include "ExecutorThread.h"
#include "../Console/CommandCode.h"
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <algorithm>
#include <pthread.h>

void ExecutorThread::listenForRequests() {
    int n_ready_fd;
    int max_fd = std::max({networkPipe.getOutputFd(), consolePipe.getOutputFd()});
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
            int command = consolePipe.readInt();
            runCommand(command);
            if(!running)
                break;
        }

        if ( FD_ISSET(networkPipe.getOutputFd(), &exception_pipes) ) {
            //TODO handle connection pipe exceptions
        }

        if ( FD_ISSET(networkPipe.getOutputFd(), &listened_pipes) ) {
            handleRequest();
        }
    }
    std::cout<<"Exe thread #"<<connection.getId()<<" exited"<<std::endl;
}

void ExecutorThread::initFdSets() {
    FD_ZERO(&listened_pipes);
    FD_ZERO(&exception_pipes);
    FD_SET(networkPipe.getOutputFd(), &listened_pipes);
    FD_SET(consolePipe.getOutputFd(), &listened_pipes);
    FD_SET(networkPipe.getOutputFd(), &exception_pipes);
    FD_SET(consolePipe.getOutputFd(), &exception_pipes);
}

void ExecutorThread::run() {
    running = true;
    mainThread = std::thread(&ExecutorThread::listenForRequests, this); //assigns the state of the started thread to mainThread, it's NOT a copy assignment
}

void ExecutorThread::join() {
    mainThread.join();
}

void ExecutorThread::handleRequest() {
     std::cout<<"Handle connection was called\n";
     int conn_id = networkPipe.readInt();
     if(conn_id < 0) {
         std::cout << "nothing to read on the networkPipe " << std::endl;
         pthread_exit(0);
     }

     std::cout<<"exe read"<<conn_id<<std::endl;
     //connCollector.enter();
     std::cout<<"exe entered\n";

     //Connection& conn = connCollector.at(conn_id);
     Request req = connection.getNextRequest();

     switch (req.getCode()) {
         case ANSW:
             connection.mockAnswer();
             break;
         default:
             break; //invalidRequestException?
     }

     std::cout<<"exe left\n";
     //connCollector.leave();
}
