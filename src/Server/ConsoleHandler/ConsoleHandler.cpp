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

void runStopThreads(int fd) {
    std::cout << "Rozpoczeto zamykanie watkow" << std::endl;
    write(fd,"kk",2);
}

void runcommand(std::string command, bool &shouldExit, int fd) {
    if ("shutdown" == command)
        runShutdown(shouldExit);
    if ("stop" == command)
	runStopThreads(fd);
}

void *handleConsole(int fd) {
  char buffer[MAX_INPUT];
  int idx = 0;
  bool shouldExit = false;

  while (!shouldExit) {
    char inputChar = getchar();
    if (idx < MAX_INPUT - 1) {
      buffer[idx] = inputChar;
      if (inputChar == '\n') {
        buffer[idx] = '\0';
        runcommand(buffer, shouldExit, fd);
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
