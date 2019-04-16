#include "ConsoleHandler.hpp"
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 20

void runShutdown(bool & shouldExit) {
    std::cout << "Rozpoczeto zamykanie serwera" << std::endl;
    shouldExit = true;
}

void runcommand(std::string command, bool &shouldExit) {
    if ("shutdown" == command)
        runShutdown(shouldExit);
}

void *handleConsole() {
  char buffer[MAX_INPUT];
  int idx = 0;
  bool shouldExit = false;

  while (!shouldExit) {
    char inputChar = getchar();
    if (idx < MAX_INPUT - 1) {
      buffer[idx] = inputChar;
      if (inputChar == '\n') {
        buffer[idx] = '\0';
        runcommand(buffer, shouldExit);
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
