//
// Created by usinkevi on 04.05.19.
//

#include "AuthMessage.h"

void AuthMessage::serialize(std::string &output) {
    if (!m_auth_msg.SerializeToString(&output))
        throw std::runtime_error("AuthMessage::serialize failed");
}

void AuthMessage::parse(const std::string &input) {
    if (!m_auth_msg.ParseFromString(input))
        throw std::runtime_error("AuthMessage::parse failed");
}

AuthMessage::AuthMessage(const std::string &input) {
    parse(input);
}

AuthMessage::AuthMessage(tinyspy::AuthMsg msg) {
    m_auth_msg = std::move(msg);
}

void AuthMessage::setMock() {
    m_auth_msg.set_login("John Doe");
    m_auth_msg.set_login("John Malkovich");
}

size_t AuthMessage::byteSize() {
    return m_auth_msg.ByteSizeLong();
}

std::string AuthMessage::debugString() const {
    return m_auth_msg.DebugString();
}

bool AuthMessage::isDefault() const {
    return google::protobuf::util::MessageDifferencer::Equals(m_auth_msg, tinyspy::AuthMsg::default_instance());
}

const int32_t AuthMessage::getType() const {
    return AuthType;
}

AuthMessage::AuthMessage() = default;

