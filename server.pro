TARGET = server

TEMPLATE = app

CONFIG += c++17

HEADERS += \
    src/Server/Client/Client.hpp \
    src/Server/ConsoleHandler/ConsoleHandler.hpp \
    src/Server/NetworkCommunication/ListenerCallAction/ListenerCallAction.hpp \
    src/Server/NetworkCommunication/ReadingAction/ReadingAction.hpp \
    src/Server/NetworkCommunication/WritingAction/WritingAction.hpp \
    src/Server/socketCreator/socketCreator.hpp

SOURCES += \
    src/Server/Client/Client.cpp \
    src/Server/ConsoleHandler/ConsoleHandler.cpp \
    src/Server/NetworkCommunication/ListenerCallAction/ListenerCallAction.cpp \
    src/Server/NetworkCommunication/ReadingAction/ReadingAction.cpp \
    src/Server/NetworkCommunication/WritingAction/WritingAction.cpp \
    src/Server/socketCreator/socketCreator.cpp \
    src/Server/main.cpp

DESTDIR = .
