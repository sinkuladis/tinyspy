//
// Created by ewelina on 15.05.19.
//
#include <iostream>
#include <cstdint>

#ifndef TINYSPY_OUTMESSAGE_H
#define TINYSPY_OUTMESSAGE_H


class OutMessage {
private:
    void *message;
    int32_t messageSize;
    int32_t offset;
public:
    OutMessage(std::string);
    ~OutMessage();
    void *get_message();
    int32_t get_messageSize();
    int32_t get_offset();
    void set_offset(int32_t);
    void set_message(std::string);
};


#endif //TINYSPY_OUTMESSAGE_H
