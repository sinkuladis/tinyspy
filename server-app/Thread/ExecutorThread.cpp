//
// Created by zmus on 18/04/19.
//
#include "ExecutorThread.h"
#include<sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include<algorithm>

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
            //TODO handle connection; do I handle it in defferent thread? or do i just handle it here? I think I should start a new thread...
            std::cout<<"executor read: "<<connectionPipe.readConnNo()<<std::endl;
        }
    }
}

void ExecutorThread::initFdSets() {
    FD_ZERO(&listened_pipes);
    FD_SET(connectionPipe.getOutputFd(), &listened_pipes);
    FD_SET(consolePipe.getOutputFd(), &listened_pipes);
    exception_pipes = listened_pipes;
}

void ExecutorThread::run() {
    mainThread = std::thread(&ExecutorThread::listenOnPipe, this); //assigns the state of the started thread to mainThread, it's NOT a copy assignment
}

void ExecutorThread::join() {
    mainThread.join();
}

