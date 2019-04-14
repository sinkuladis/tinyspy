#include "ConsoleHandler.hpp"
#include "../SignalsHandler/SignalHandler.hpp"
#include <iostream>


void handleConsole() {
    while(true) {
        std::string command;
        std::cin >> command;

        if( command == "shutdown" )
            exit(1);
    }
}
