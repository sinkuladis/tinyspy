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
    connectInputPipe.writeInt(SHUT);
    connMgr.shutdownAllNow();
    running = false;
}