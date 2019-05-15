//
// Created by ewelina on 15.05.19.
//

#include "OutMessage.h"

OutMessage::OutMessage(std::string message) {
    this->messageSize = message.size();
    this->offset = 0;

    char *buffer = (char *)malloc(sizeof(char) * message.size());
    for (int32_t i = 0; i < this->messageSize ; i++)
        buffer[i] = message[i];
    this->message = (void *)buffer;
}

OutMessage::~OutMessage() {
    free(this->message);
}

void *OutMessage::get_message() {
    return this->message;
}

int32_t OutMessage::get_messageSize() {
    return this->messageSize;
}

int32_t OutMessage::get_offset() {
    return this->offset;
}

void OutMessage::set_offset(int32_t new_offset) {
    if (new_offset < this->messageSize)
        this->offset =  new_offset;
    else
        this->offset = this->messageSize - 1;
}

void OutMessage::set_message(std::string message) {
    free(this->message);
    this->messageSize = message.size();
    this->offset = 0;

    char *buffer = (char *)malloc(sizeof(char) * message.size());
    for (int32_t i = 0; i < this->messageSize ; i++)
        buffer[i] = message[i];
    this->message = (void *)buffer;
}