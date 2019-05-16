//
// Created by ewelina on 15.05.19.
//
#include <list>
#include "monitor.h"
#include <iostream>
#include "../OutMessage/OutMessage.h"

#ifndef TINYSPY_OUTMESSAGEQUEUE_H
#define TINYSPY_OUTMESSAGEQUEUE_H


class OutMessageQueue {
private:
    std::list<OutMessage> messages;
    int32_t messages_num;
    Semaphore messages_mutex;
    Semaphore messages_num_mutex;
public:
    OutMessageQueue();
    ~OutMessageQueue();
    void add_message(OutMessage);
    OutMessage *get_message();
    std::string get_string();
};


#endif //TINYSPY_OUTMESSAGEQUEUE_H
