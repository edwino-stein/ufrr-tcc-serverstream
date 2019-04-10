#include "ws/Server.hpp"
#include "ws/Session.hpp"
#include "ws/ServerListener.hpp"

using namespace ws;
using runtime::LoopInside;
using exceptions::Exception;

Server::Server(const int port, ws::ServerListener * const listener, const bool autoClearup):
LoopInside(), ServerRequestAcceptor(port, listener), autoClearup(autoClearup){}

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

    this->sessionsMtx.lock();

    Vector<Session *>::iterator it = this->sessions.begin();
    while(it != this->sessions.end()){
        Session *s = *it;

        if(s->readyState == SessionLifeCycle::CLOSED){
            this->sessions.erase(it);
            delete s;
        }

        it++;
    }

    this->sessionsMtx.unlock();
}
