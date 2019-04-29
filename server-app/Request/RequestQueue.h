//
// Created by zmus on 29/04/19.
//

#ifndef TINYSPY_REQUESTQUEUE_H
#define TINYSPY_REQUESTQUEUE_H


#include <list>
#include <mutex>
#include <condition_variable>
#include "Request.h"
#include "RequestQueueState.h"

class RequestQueue {
private:
    std::list<Request> queue;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock;
    std::condition_variable empty;
    int state;
public:
    RequestQueue()
    : queue(),
    mutex(),
    lock(mutex),
    state(RUNNING)
    {}

    bool isRunning();
    void shutdownImmediately();
    Request getNext();
    void enqueue(Request req);

};


#endif //TINYSPY_REQUESTQUEUE_H
