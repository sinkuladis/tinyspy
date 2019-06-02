//
// Created by zmus on 20/04/19.
//

#include "Thread.h"
#include "../Console/CommandCode.h"

void Thread::shutdown() {
    running = false;
}

void Thread::runCommand(int commandCode) {
    switch(commandCode) {
        case UNAVAILABLE: break;
        case SHUT: shutdown(); break;
    }
}
