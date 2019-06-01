//
// Created by ewelina on 15.05.19.
//

#include "OutMessageQueue.h"
#include <iostream>

OutMessageQueue::OutMessageQueue(): messages_num(0), mutex(1) {}
OutMessageQueue::~OutMessageQueue() {
    this->messages.clear();
}

void OutMessageQueue::add_message(OutMessage new_message) {
    this->mutex.p();
    this->messages_num++;
    this->messages.push_back(std::move(new_message));
    this->mutex.v();
}


void *OutMessageQueue::get_message() {
    this->mutex.p();
    void* message = this->messages.front().get_message();
    this->mutex.v();
    return message;
}


OutMessage* OutMessageQueue::get_out_message() {
    this->mutex.p();
    OutMessage* out_message = &(this->messages.front());
    this->mutex.v();
    return out_message;
}


int32_t OutMessageQueue::get_messages_num() {
    this->mutex.p();
    int32_t num = this->messages_num;
    this->mutex.v();
    return num;
}


int32_t OutMessageQueue::get_message_size() {
    this->mutex.p();
    int32_t offset = this->messages.front().get_message_size();
    this->mutex.v();
    return offset;
}


int32_t OutMessageQueue::get_offset() {
    this->mutex.p();
    int32_t offset = this->messages.front().get_offset();
    this->mutex.v();
    return offset;
}


int32_t OutMessageQueue::push_offset(int32_t difference) {
    // Funkcja zwraca liczbe bajtow, jakich nie udalo sie przeczytac
    this->mutex.p();
    int32_t new_offset = this->messages.front().get_offset() + difference;
    int32_t remaining_data = this->messages.front().get_message_size() - new_offset;
    if(remaining_data > 0)
        this->messages.front().set_offset(new_offset);
    else
        this->messages.pop_front();
    this->mutex.v();

    return remaining_data;
}

int32_t OutMessageQueue::not_empty() const {

    if (this->messages.empty())
    return false;

    return true;
}
