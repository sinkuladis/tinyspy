#include "ConsoleHandler.hpp"
#include <iostream>


void handleConsole() {
    while(true) {
        std::string command;
        std::cin >> command;

        if( command == "shutdown" )
            exit(1);
    }
}
