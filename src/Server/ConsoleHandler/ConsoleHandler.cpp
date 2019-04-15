#include "ConsoleHandler.hpp"
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 20

void runShutdown[[noreturn]](std::promise<void> &exitSignal) {
  std::cout << "Rozpoczeto zamykanie serwera" << std::endl;
  exitSignal.set_value();
  sleep(1);
  std::cout << "Wylaczanie obslugi konsoli" << std::endl;
  pthread_exit(nullptr);
}

void runcommand(std::string command, std::promise<void> &exitSignal) {
  if ("shutdown" == command)
    runShutdown(exitSignal);
}

void handleConsole(std::promise<void> &exitSignal) {
  char buffer[MAX_INPUT];
  int idx = 0;

  while (true) {
    char inputChar = getchar();
    if (idx < MAX_INPUT - 1) {
      buffer[idx] = inputChar;
      if (inputChar == '\n') {
        buffer[idx] = '\0';
        runcommand(buffer, exitSignal);
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
