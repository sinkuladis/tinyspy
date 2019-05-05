//
// Created by usinkevi on 04.05.19.
//

#ifndef TINYSPY_DATAMESSAGE_H
#define TINYSPY_DATAMESSAGE_H


#include "messages.pb.h"
#include "TinyMessage.h"
#include <google/protobuf/util/message_differencer.h>
#include <limits>

class DataMessage: public TinyMessage {
public:
    DataMessage();
    DataMessage(std::string &input);
    DataMessage(tinyspy::DataMsg data_msg);

    void serialize(std::string &output) override;
    void parse(const std::string &input) override;

    void setMock();
    void setText(std::string &text);

    int getNum() const;
    const int32_t getType() const override;

    size_t byteSize() override;
    std::string debugString() const override;

    bool isDefault() const override;

    void clear();

private:
    tinyspy::DataMsg m_data_msg;
};


#endif //TINYSPY_DATAMESSAGE_H
