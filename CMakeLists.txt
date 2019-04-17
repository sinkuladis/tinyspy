cmake_minimum_required(VERSION 3.13)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

add_executable(Server.out server-app/main.cpp
        server-app/Connection/Connection.h server-app/Connection/Connection.cpp
        server-app/Connection/ClientConnection.h server-app/Connection/ClientConnection.cpp
        server-app/Connection/ConnectionCollector.h server-app/Connection/ConnectionCollector.cpp
        server-app/Thread/ConnectionThread.h server-app/Thread/ConnectionThread.cpp
        server-app/Server.h server-app/Server.cpp)