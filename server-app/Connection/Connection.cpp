//
// Created by zmus on 16/04/19.
//

#include <cstdlib>
#include <iterator>
#include <Serialization/AuthMessage.h>
#include "Connection.h"
#include "executor_args.h"
#include "../Exception/ConnectionTerminationException.h"
#include "Serialization/messages.pb.h"
#include "ConnectionState.h"

Connection::Connection(Socket nSock) : in_buffer(1024, 0), out_buffer(1024, 0) {
    sock = nSock;
    state = ONGOING;
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

//    std::cout << ">>>>>> Debug Message String\n" << msg->debugString() << "<<<<<<" << std::endl;

    int req = readbytes == 0 ? TERM : ANSW; // deserializacja na miare makeshiftu XD

    if (req == TERM)
        throw ConnectionTerminationException(getId());
//        std::cerr << "Read 0 bytes" << std::endl;
    else
        requestQueue.enqueue(Request(req));
}

void Connection::writeDataToSend(char *data) {
    sock.write(data, 0);
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

void *Connection::executor_routine(void *args_) {
    struct executor_args *args = static_cast<executor_args *>(args_);
    ConnectionManager &connMgr = *(args->connMgr);
    Connection conn(args->sock);
    free(args);

    connMgr.collect(conn);

    while (conn.state == ONGOING) {
        //w tym momencie polecenia z konsolki dotyczące stanu połączenia będą obsługiwane jako requesty i wszystko zostaje ułatwione 500x
        Request r = conn.requestQueue.getNext();
        conn.handleRequest(r);
    }

    connMgr.unregister(conn.getId());
}

void Connection::handleRequest(Request request) {
    int reqcode = request.getCode();
    switch (reqcode) {
        case ANSW:
            mockAnswer();
            break;
        case TERM:
            terminate();
            break;
        default:
            //InvalidRequestException?
            break;
    }
}

void Connection::terminate() {
    state = SHUTDOWN;
}

RequestQueue &Connection::getRequestQueue() {
    return requestQueue;
}

