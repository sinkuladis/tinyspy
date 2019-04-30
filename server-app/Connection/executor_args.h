//
// Created by zmus on 30/04/19.
//

#ifndef TINYSPY_EXECUTOR_ARGS_H
#define TINYSPY_EXECUTOR_ARGS_H

#include "ConnectionManager.h"

struct executor_args {
    ConnectionManager connMgr;
    Socket sock;
};

#endif //TINYSPY_EXECUTOR_ARGS_H
