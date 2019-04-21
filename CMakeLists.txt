cmake_minimum_required(VERSION 3.13)
project(tinyspy)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -pthread")

add_executable(Server.out server-app/main.cpp
        server-app/Pipes/Pipe.h server-app/Pipes/Pipe.cpp
        server-app/Socket/Socket.h server-app/Socket/Socket.cpp
        server-app/Connection/Connection.h server-app/Connection/Connection.cpp
        server-app/Connection/ConnectionCollector.h server-app/Connection/ConnectionCollector.cpp
        server-app/Thread/ConnectionThread.h server-app/Thread/ConnectionThread.cpp
        server-app/Server.h server-app/Server.cpp
        server-app/Thread/ExecutorThread.cpp server-app/Thread/ExecutorThread.h
        server-app/Request/Request.cpp server-app/Request/Request.h
        server-app/Request/RequestCode.h
        server-app/Console/ConsoleHandler.cpp server-app/Console/ConsoleHandler.h
        server-app/Console/CommandCode.h
        server-app/Thread/Thread.cpp server-app/Thread/Thread.h)