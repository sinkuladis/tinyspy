TARGET = client

TEMPLATE = app

CONFIG += c++17

HEADERS += \
    src/Client/ClientConnection/clientconnection.h

SOURCES += \
    src/Client/ClientConnection/clientconnection.cpp \
    src/Client/client.cpp

DESTDIR = .
