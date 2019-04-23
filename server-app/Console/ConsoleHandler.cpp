//
// Created by zmus on 20/04/19.
//

#include <iostream>
#include "ConsoleHandler.h"
#include "CommandCode.h"

void ConsoleHandler::handleConsole() {
    char buffer[MAX_INPUT];
    int idx = 0;

    while (running) {
        char inputChar = getchar();
        if (idx < MAX_INPUT - 1) {
            buffer[idx] = inputChar;

            if (inputChar == '\n') {
                buffer[idx] = '\0';
                runCommand(buffer);
                idx = 0;
            } else
                idx++;

        } else {
            std::cout << "Maksymalna dlugosc komendy wynosi " << MAX_INPUT;
            std::cout << ". Reset komendy" << std::endl;
            idx = 0;
        }
    }
}

void ConsoleHandler::runCommand(std::string command) {
    if ("shutdown" == command)
        runShutdown();
}


void ConsoleHandler::runShutdown() {
    std::cout << "Rozpoczeto zamykanie serwera" << std::endl;
    //executorInputPipe.writeInt(SHUTDOWN); //chyba prościej będzie już z przekazywaniem stringów... i będziemy mieli ogarnięte badanie długości
    connectInputPipe.writeInt(SHUTDOWN);
    connMgr.notifyAll(SHUTDOWN);
    //it'll take a while so we do it in second order so that we won't be trying to accept any incoming connections at the time being
    //FIXME though, if we're doing it like that, we should leave the monitor only after going through the while select loop in the networkThread
    //otherwise, we could leave the monitor, allow for a executionThread to shut down and then register an incoming packet on a socket that has already been closed
    running = false;
}