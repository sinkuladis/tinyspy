//
// Created by zmus on 20/04/19.
//

#ifndef TINYSPY_CONSOLEHANDLER_H
#define TINYSPY_CONSOLEHANDLER_H


#include "../Pipes/Pipe.h"

class ConsoleHandler {
private:
    static const int MAX_INPUT = 20;
    Pipe& executorInputPipe;
    Pipe& connectInputPipe;
    bool running;

    void runCommand(std::string);
    void runShutdown();
public:
    ConsoleHandler(Pipe& connectP, Pipe& executorP)
    : executorInputPipe(executorP),
      connectInputPipe(connectP),
      running(true)
    {}

    void handleConsole();
};


#endif //TINYSPY_CONSOLEHANDLER_H
