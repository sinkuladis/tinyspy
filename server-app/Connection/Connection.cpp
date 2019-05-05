//
// Created by zmus on 16/04/19.
//

#include <cstdlib>
#include <iterator>
#include <Serialization/AuthMessage.h>
#include "Connection.h"
#include "../Exception/ConnectionTerminationException.h"
#include "Serialization/messages.pb.h"

Connection::Connection(Socket nSock) : in_buffer(1024, 0), out_buffer(1024, 0) {
    sock = nSock;
}

void Connection::readReceivedData() {
    int32_t msgLen = 0, msgType = 0;
    sock.read(&msgType);
    msgType = ntohl(msgType);
    sock.read(&msgLen);
    msgLen = ntohl(msgLen);
    in_buffer.clear();
    in_buffer.resize(msgLen);
    int readbytes = sock.read(in_buffer.data(), msgLen);

    std::string input(in_buffer.begin(), in_buffer.end());

    std::unique_ptr<TinyMessage> msg;

    if (msgType == DataType)
        msg = std::make_unique<DataMessage>(input);
    else if (msgType == AuthType)
        msg = std::make_unique<AuthMessage>(input);

//    std::cout << msg->debugString() << std::endl;

    int req = readbytes == 0 ? TERM : ANSW; // deserializacja na miare makeshiftu XD
    if (req == TERM)
        throw ConnectionTerminationException(getId());
    else
        requestQueue.push_back(Request(req));
}

void Connection::writeDataToSend(char *data) {
    sock.write(data, 0);
}

Request Connection::getNextRequest() {
    Request req = requestQueue.front();
    requestQueue.pop_front();
    return req;
}

void Connection::mockAnswer() {
    std::string mockAnswer("Howdy, client #" + std::to_string(getId()));

    std::string output;
    DataMessage dataMsg;
    dataMsg.setMock();
    dataMsg.setText(mockAnswer);
    dataMsg.serialize(output);
    output.append("\0");

    int32_t msgLen = ntohl(output.length());
    int32_t msgType = ntohl(dataMsg.getType());
    sock.write(&msgType);
    sock.write(&msgLen);
    sock.write(output.data(), output.length());
}
