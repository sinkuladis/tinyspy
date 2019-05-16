//
// Created by ewelina on 15.05.19.
//

#include "OutMessageQueue.h"
#include <iostream>

OutMessageQueue::OutMessageQueue(): messages_num(0), messages_mutex(1), messages_num_mutex(1) {}
OutMessageQueue::~OutMessageQueue() {
    this->messages.clear();
}

void OutMessageQueue::add_message(OutMessage new_message) {
    this->messages_mutex.p();
    this->messages.push_back(std::move(new_message));
    this->messages_mutex.v();

    this->messages_num_mutex.p();
    this->messages_num++;
    this->messages_num_mutex.v();
}

OutMessage* OutMessageQueue::get_message() {
    this->messages_mutex.p();
    OutMessage* out_message = &(this->messages.front());
    this->messages_mutex.v();
    return out_message;
}

std::string OutMessageQueue::get_string() {
    return (char*)(this->messages.front()).get_message();
}