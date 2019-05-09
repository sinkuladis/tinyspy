//
// Created by usinkevi on 04.05.19.
//

#ifndef TINYSPY_AUTHMESSAGE_H
#define TINYSPY_AUTHMESSAGE_H

#include "messages.pb.h"
#include "TinyMessage.h"
#include <google/protobuf/util/message_differencer.h>

class AuthMessage: public TinyMessage {
public:
    AuthMessage();

    AuthMessage(const std::string &input);

    AuthMessage(tinyspy::AuthMsg msg);

    void serialize(std::string &output) override;

    void parse(const std::string &input) override;

    size_t byteSize() override;

    void setMock();

    std::string debugString() const override;

    bool isDefault() const override;

    const int32_t getType() const override;

private:
    tinyspy::AuthMsg m_auth_msg;
};


#endif //TINYSPY_AUTHMESSAGE_H
