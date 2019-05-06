cmake_minimum_required(VERSION 3.13)
project(tinyspy)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -pthread")

include_directories(server-app)

link_directories(3rdparty)

find_package(Protobuf REQUIRED)

add_executable(Server.out server-app/main.cpp
        server-app/Pipes/Pipe.h server-app/Pipes/Pipe.cpp
        server-app/Socket/Socket.h server-app/Socket/Socket.cpp
		server-app/Socket/ListeningSocket.h server-app/Socket/ListeningSocket.cpp
		server-app/Connection/Connection.h server-app/Connection/Connection.cpp
        server-app/Connection/ConnectionManager.h server-app/Connection/ConnectionManager.cpp
        server-app/Thread/NetworkThread.h server-app/Thread/NetworkThread.cpp
        server-app/Server.h server-app/Server.cpp
        server-app/Request/Request.cpp server-app/Request/Request.h
        server-app/Request/RequestCode.h
        server-app/Console/ConsoleHandler.cpp server-app/Console/ConsoleHandler.h
        server-app/Console/CommandCode.h
        server-app/Thread/Thread.cpp server-app/Thread/Thread.h
        server-app/Exception/ConnectionException.h server-app/Exception/ConnectionException.cpp
        server-app/Exception/ConnectionTerminationException.h server-app/Exception/ConnectionTerminationException.cpp
        server-app/Serialization/messages.pb.h server-app/Serialization/messages.pb.cc
        server-app/Serialization/AuthMessage.h server-app/Serialization/AuthMessage.cpp
        server-app/Serialization/DataMessage.cpp server-app/Serialization/DataMessage.cpp
        server-app/Serialization/TinyMessage.h server-app/Serialization/TinyMessage.h
        server-app/basic/RWOperation.cpp server-app/basic/RWOperation.h
		server-app/Request/RequestQueue.h server-app/Request/RequestQueue.cpp
		server-app/Connection/ConnectionState.h
        server-app/Connection/executor_args.h)

add_executable(Client.out client-app/client.cpp
        server-app/Serialization/messages.pb.h server-app/Serialization/messages.pb.cc
        server-app/Serialization/AuthMessage.h server-app/Serialization/AuthMessage.cpp
        server-app/Serialization/DataMessage.cpp server-app/Serialization/DataMessage.cpp
        server-app/Serialization/TinyMessage.h server-app/Serialization/TinyMessage.h)

target_link_libraries(Server.out libprotobuf.a)
target_link_libraries(Client.out libprotobuf.a)

