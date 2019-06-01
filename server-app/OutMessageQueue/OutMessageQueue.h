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
    Semaphore mutex;
public:
    OutMessageQueue();
    ~OutMessageQueue();
    void add_message(OutMessage);
    void* get_message();
    OutMessage *get_out_message();
    int32_t get_offset();
    int32_t get_message_size();
    int32_t get_messages_num();
    int32_t push_offset(int32_t);
    int32_t not_empty() const;
};


#endif //TINYSPY_OUTMESSAGEQUEUE_H
