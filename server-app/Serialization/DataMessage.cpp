//
// Created by usinkevi on 04.05.19.
//

#include "DataMessage.h"

void DataMessage::serialize(std::string &output) {
    if (!m_data_msg.SerializeToString(&output))
        throw std::runtime_error("DataMessage::serialize failed");
}

void DataMessage::parse(const std::string &input) {
    m_data_msg.ParseFromString(input);
}

DataMessage::DataMessage(std::string &input) {
    parse(input);
}

DataMessage::DataMessage(tinyspy::DataMsg data_msg) {
    m_data_msg = data_msg;
}

void DataMessage::setMock() {
    m_data_msg.set_request_id(std::numeric_limits<int32_t>::lowest());
    m_data_msg.set_agent_id(std::numeric_limits<int32_t>::lowest());
    m_data_msg.set_live_data(false);
    m_data_msg.set_session_token(std::numeric_limits<int32_t>::lowest());
    m_data_msg.set_num(-1);
}

size_t DataMessage::byteSize() {
    return m_data_msg.ByteSizeLong();
}

bool DataMessage::isDefault() const {
    return google::protobuf::util::MessageDifferencer::Equals(m_data_msg, tinyspy::DataMsg::default_instance());
}

std::string DataMessage::debugString() const {
    return m_data_msg.DebugString();
}

int DataMessage::getNum() const {
    return m_data_msg.num();
}

void DataMessage::clear() {
    m_data_msg.Clear();
}

const int32_t DataMessage::getType() const {
    return DataType;
}

void DataMessage::setText(std::string &text) {
    m_data_msg.set_text(text);
}

DataMessage::DataMessage() = default;

