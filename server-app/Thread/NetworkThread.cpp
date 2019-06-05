//
// Created by zmus on 16/04/19.
//

#include <pthread.h>

#include <sys/select.h>
#include <unistd.h>

#include <functional>
#include <algorithm>

#include "../Connection/executor_args.h"
#include "NetworkThread.h"
#include "Exception/ConnectionTerminationException.h"


void NetworkThread::run() {
    run_mutex.lock();
    running = true;
    pthread_create(&thread_id, NULL, _run_net_routine, this);
}

void NetworkThread::join() {
    pthread_join(thread_id, NULL);
    run_mutex.unlock();
}


void* NetworkThread::_run_net_routine(void *netThreadPtr) {
    NetworkThread &netThread = *((NetworkThread*)netThreadPtr);
    netThread._net_routine();
    return nullptr;
}

void NetworkThread::_net_routine() {
    connMgr.setFdSets(&listened_fdset, &write_fdset, &exception_fdset);
    int nfds, max_fd;
    struct timeval timeout;
    timeout = {
            .tv_sec = 0,
            .tv_usec = 0
    };
    while (running) {
        timeout = listenSock.initialize(timeout);
        if (listenSock.isReady()) {
            max_fd = initFdSets();
            nfds = select(max_fd + 1, &listened_fdset, &write_fdset, &exception_fdset, NULL);
        } else
            nfds = select(max_fd + 1, &listened_fdset, &write_fdset, &exception_fdset, &timeout);

        if (nfds < 0 || !listenSock.isReady()) {
            //TODO handle error
        } else {
            if (FD_ISSET(consolePipe.getOutputFd(), &exception_fdset)) {
                //TODO handle console exceptions
            }
            if (FD_ISSET(consolePipe.getOutputFd(), &listened_fdset)) {
                int command = consolePipe.readInt();
                runCommand(command);
                if (!running)
                    break;
            }
            if (FD_ISSET(listenSock.getSockFd(), &exception_fdset)) {
                //TODO handle listenSock exceptions
            }
            if (FD_ISSET(listenSock.getSockFd(), &listened_fdset))
                acceptNewConnection();

            connMgr.readAll();
            connMgr.writeAll();
        }
    }
    std::cout << "Connection thread exited" << std::endl;
}

void NetworkThread::acceptNewConnection() {
    Socket newSock = listenSock.accept();
    int connection_id = newSock.getSockFd();
    struct executor_args *args = static_cast<executor_args*>(calloc(1,sizeof(struct executor_args)));
    args->connMgr = &connMgr;
    args->sock = newSock;

    Connection& newConnection = Connection::startExecutor(args);
    connMgr.collect(newConnection);
    std::cout << "Added client #" << connection_id << std::endl;
}

int NetworkThread::initFdSets() {
    int max_fd = connMgr.getConnectionsFdSet();

    int listen_sock_fd = listenSock.getSockFd();
    int console_fd = consolePipe.getOutputFd();
    FD_SET(listen_sock_fd, &listened_fdset);
    FD_SET(console_fd, &listened_fdset);
    FD_SET(listen_sock_fd, &exception_fdset);
    FD_SET(console_fd, &exception_fdset);

    max_fd = std::max({max_fd, listen_sock_fd, console_fd});

    return max_fd;
}


