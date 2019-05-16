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
    int32_t message_size;
    int32_t offset;
public:
    OutMessage(std::string);
    OutMessage(OutMessage&&);
    ~OutMessage();
    void *get_message();
    int32_t get_message_size();
    int32_t get_offset();
    void set_offset(int32_t);
    void set_message(std::string);

    OutMessage& operator=(OutMessage&&);
};


#endif //TINYSPY_OUTMESSAGE_H
