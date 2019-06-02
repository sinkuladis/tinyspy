//
// Created by ewelina on 15.05.19.
//

#include "OutMessage.h"

OutMessage::OutMessage(std::string message) {
    this->message_size = message.size();
    this->offset = 0;

    char *buffer = (char *)malloc(sizeof(char) * message.size() + 1);
    for (int32_t i = 0; i < this->message_size ; i++)
        buffer[i] = message[i];
    buffer[message.size()] = '\0';
    this->message = (void *)buffer;
}

OutMessage::OutMessage(OutMessage&& other) : message(std::move(other.message)) {
    this->message_size = other.get_message_size();
    this->offset = other.get_offset();
    other.message = nullptr;
}

OutMessage& OutMessage::operator=(OutMessage&& other) {
    free(this->message);
    this->message = std::move(other.message);
    this->message_size = other.message_size;
    this->offset = other.offset;
}


OutMessage::~OutMessage() {
    free(this->message);
}

void *OutMessage::get_message() {
    return this->message;
}

int32_t OutMessage::get_message_size() {
    return this->message_size;
}

int32_t OutMessage::get_offset() {
    return this->offset;
}

void OutMessage::set_offset(int32_t new_offset) {
    if (new_offset < this->message_size)
        this->offset =  new_offset;
    else
        this->offset = this->message_size - 1;
}

void OutMessage::set_message(std::string message) {
    free(this->message);
    this->message_size = message.size();
    this->offset = 0;

    char *buffer = (char *)malloc(sizeof(char) * message.size());
    for (int32_t i = 0; i < this->message_size ; i++)
        buffer[i] = message[i];
    this->message = (void *)buffer;
}