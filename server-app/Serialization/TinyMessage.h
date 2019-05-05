//
// Created by usinkevi on 05.05.19.
//

#ifndef TINYSPY_TINYMESSAGE_H
#define TINYSPY_TINYMESSAGE_H

#include <string>
#include "messages.pb.h"

enum MessageType {AuthType, DataType};

class TinyMessage {
public:
    virtual ~TinyMessage() = default;

    virtual void serialize(std::string &output) = 0;

    virtual void parse(const std::string &input) = 0;

    virtual size_t byteSize() = 0;

    virtual std::string debugString() const = 0;

    virtual bool isDefault() const = 0;

    virtual const int32_t getType() const = 0;
};

#endif //TINYSPY_TINYMESSAGE_H
