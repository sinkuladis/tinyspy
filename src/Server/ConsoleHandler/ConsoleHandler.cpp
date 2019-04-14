#include "ConsoleHandler.hpp"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 20


void runcommand(std::string command) {
    if("shutdown" == command) {
        std::cout << "Zamykanie serwera" << std::endl;
        exit(0);
    }
    
}

void handleConsole() {
    char buffer[MAX_INPUT];
    int idx = 0;

    while(true) {
        char inputChar = getchar();
        if( idx <  MAX_INPUT - 1) {
            buffer[idx] = inputChar;
            if (inputChar == '\n') {
                buffer[idx] = '\0';
                runcommand(buffer);
                idx = 0;
            }
            else
                idx++;
        }
        else {
            std::cout << "Maksymalna dlugosc komendy wynosi " << MAX_INPUT;
            std::cout << ". Reset komendy" << std::endl;
            idx = 0;
        }
    }
}
