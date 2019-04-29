//
// Created by zmus on 29/04/19.
//

#include "RequestQueue.h"

Request RequestQueue::getNext() {
    lock.lock();
    if(queue.size() == 0)
        empty.wait(lock); //powinno odzyskać locka automatycznie
    Request req = queue.front();
    queue.pop_front();
    lock.unlock();
    return req;
}

void RequestQueue::enqueue(Request req) {
    lock.lock();
    queue.push_back(req);
    if(queue.size() == 1)
        empty.notify_one(); //tylko jeden thread będzie czekał
    lock.unlock();
}

bool RequestQueue::isRunning() {
    lock.lock();
    bool ret = state == RUNNING;
    lock.unlock();
    return ret;
}

void RequestQueue::shutdownImmediately() {
    lock.lock();
    queue.push_front(Request(TERM));
    if(queue.size() == 1)
        empty.notify_one();
    lock.unlock();
}

