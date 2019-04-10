#include "ws/Session.hpp"
#include "ws/SessionListener.hpp"

using namespace ws;

using runtime::LoopInside;
using exceptions::WsErrorException;
using boost::system::system_error;
using boost::system::error_code;

Session::Session(WSocket * const socket, SessionListener * const listener) :
LoopInside(), listener(listener), socket(socket), readyState(_readyState), type(_type) {
    this->_readyState = SessionLifeCycle::CONNECTING;
    this->setType(SessionMessageType::TEXT);
    this->isDetach = false;
}

Session::~Session(){
    if(this->socket != NULL) this->close();
}

void Session::loop(){

    if(this->readyState != SessionLifeCycle::OPEN) return;

    IOBuffer buffer;

    try{
        if(this->socket->read(buffer) > 0) this->listener->onMessage(this, buffer);
    }
    catch(system_error const& se){

        if(se.code() != websocket::error::closed){
            this->listener->onError(this, WsErrorException(se));
        }
        else{
            this->_readyState = SessionLifeCycle::CLOSING;
            this->listener->onClose(this, se.code().value());
            this->close();
        }

        return;
    }
    catch(std::exception const& e){
        return;
    }
}

void Session::run(){
    this->_readyState = SessionLifeCycle::OPEN;
    LoopInside::run();
}

void Session::stop(const bool join){
    if(!this->isDetach){
        this->isDetach = true;
        this->pThread.detach();
        LoopInside::stop(join);
    }
}

void Session::close(SessionCloseCode code){

    if(this->readyState == SessionLifeCycle::CLOSED) return;

    this->_readyState = SessionLifeCycle::CLOSING;

    if(this->socket != NULL){
        delete this->socket;
        this->socket = NULL;
    }

    this->_readyState = SessionLifeCycle::CLOSED;
    this->stop(false);
}

void Session::send(IOBuffer &data){

    if(this->readyState != SessionLifeCycle::OPEN) return;

    try{
        this->socket->write(data.data());
    }
    catch(system_error const& e){
        this->listener->onError(this, WsErrorException(e));
    }
}

void Session::setType(SessionMessageType type){

    if(!(this->readyState == SessionLifeCycle::OPEN || this->readyState == SessionLifeCycle::CONNECTING)) return;

    switch(type){

        case SessionMessageType::BINARY:
            this->socket->text(false);
            this->socket->binary(true);
            this->_type = SessionMessageType::BINARY;
        break;

        default:
        case SessionMessageType::TEXT:
            this->socket->binary(false);
            this->socket->text(true);
            this->_type = SessionMessageType::TEXT;
        break;
    }
}
