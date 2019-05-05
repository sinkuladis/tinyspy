//
// Created by zmus on 16/04/19.
//

#ifndef TINYSPY_CONNECTION_H
#define TINYSPY_CONNECTION_H


#include <iostream>
#include <list>
#include <vector>
#include "../Socket/Socket.h"
#include "../Request/Request.h"
#include "Serialization/DataMessage.h"

class Connection {
protected:
    Socket sock;
    std::vector<char> in_buffer;
    std::vector<char> out_buffer;
    std::list<Request> requestQueue;
public:
    Connection(Socket nSock);
    ~Connection() { sock.shut(); }

    //TODO tutaj bedzie trzeba przeczytac
    //1* rozmiar odebranej zserializowanej wiadomosci
    //2* cala jej reszte
    //obie rzeczy z uzyciem metody z klasy Socket, ktora zadba o przeczytanie zadanej liczby bajtow za kazdym razem
    //tak, ze to odpalac bedziemy w NetworkThread
    //a nastepnie bedziemy dawac znac warstwie deserializujacej, ze zadane connection przyslalo dane do deserializacji
    void readReceivedData();
    void mockAnswer();

    void writeDataToSend(char*);
    void printMessage() {std::cout<<"Client #"<<getId()<<" said "<< in_buffer.data()<<std::endl;}

    Socket getSock() { return sock;}

    //testowa metoda, tymczasowo id polaczenia to zwiazany z jego gniazdem file descriptor, unikalny dla polaczenia w trakcie jego dzialania
    int getId() {return sock.getSockFd(); }

    Request getNextRequest();
};


#endif //TINYSPY_CONNECTION_H
