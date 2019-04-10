#include "ws/Server.hpp"
#include "ws/Session.hpp"
#include "ws/ServerListener.hpp"

#include <algorithm>

using namespace ws;
using runtime::LoopInside;
using exceptions::Exception;

Server::Server(const int port, ws::ServerListener * const listener):
LoopInside(), ServerRequestAcceptor(port, listener){}

Server::~Server(){
    this->stop();
}

void Server::stop(){

    if(!this->running) return;

    ServerRequestAcceptor::close();
    LoopInside::stop(true);


    this->sessionsMtx.lock();

    for(Vector<Session *>::iterator it = this->sessions.begin(); it != this->sessions.end(); ++it){
        delete (*it);
    }

    this->sessions.clear();
    this->sessionsMtx.unlock();
}

void Server::loop(){

    Session *session = this->accept();

    if(session != NULL){
        this->sessionsMtx.lock();
        this->sessions.push_back(session);
        this->sessionsMtx.unlock();

        session->run();
        this->listener->onConnection(session);
    }

    this->clearup();
}

void Server::broadcast(IOBuffer &data){
    this->sessionsMtx.lock();
    for(Vector<Session *>::iterator it = this->sessions.begin(); it != this->sessions.end(); ++it){
        (*it)->send(data);
    }
    this->sessionsMtx.unlock();
}

void Server::clearup(){

    if(!this->running) return;
    Vector<Session *> toFree;

    this->sessionsMtx.lock();
    this->sessions.erase(
        std::remove_if(
            this->sessions.begin(),
            this->sessions.end(),
            [&toFree](Session * s){
                if(s->readyState != SessionLifeCycle::CLOSED) return false;
                toFree.push_back(s);
                return true;
            }
        ),
        this->sessions.end()
    );

    this->sessionsMtx.unlock();

    for(Vector<Session *>::iterator it = toFree.begin(); it != toFree.end(); ++it){
        delete (*it);
    }

    toFree.clear();
}
