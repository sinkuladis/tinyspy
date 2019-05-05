//
// Created by zmus on 20/04/19.
//

#ifndef TINYSPY_CONSOLEHANDLER_H
#define TINYSPY_CONSOLEHANDLER_H


#include "../Pipes/Pipe.h"
#include "../Connection/ConnectionManager.h"

class ConsoleHandler {
private:
    static const int MAX_INPUT_SIZE = 20;
    ConnectionManager& connMgr;
    Pipe& connectInputPipe;
    bool running;

    void runCommand(std::string);
    void runShutdown();
public:
    ConsoleHandler(Pipe& connectP, ConnectionManager& nConnMgr)
    : //executorInputPipe(executorP),
      connMgr(nConnMgr),
      connectInputPipe(connectP),
      running(true)
    {}

    void handleConsole();
};


#endif //TINYSPY_CONSOLEHANDLER_H
