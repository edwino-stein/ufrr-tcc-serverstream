#include "ws/Session.hpp"
#include "ws/SessionListener.hpp"

using namespace ws;

using runtime::LoopInside;
using exceptions::Exception;

Session::Session(WSocket * const socket, SessionListener * const listener) :
LoopInside(), listener(listener), socket(socket), readyState(_readyState), type(_type) {
    this->setType(SessionMessageType::TEXT);
    this->_readyState = SessionLifeCycle::CONNECTING;
}

Session::~Session(){
    this->close(SessionCloseCode::normal);
    this->stop(true);
    delete this->socket;
}

void Session::loop(){

    if(this->readyState != SessionLifeCycle::OPEN) return;

    IOBuffer buffer;

    try{
        this->socket->read(buffer);
    }
    catch(boost::system::system_error const& se){

        if(se.code() != websocket::error::closed)
            this->listener->onError(this, Exception(se));
        else
            this->listener->onClose(this, se.code().value());

        return;
    }
    catch(std::exception const& e){
        this->listener->onError(this, Exception(e));
        return;
    }

    this->listener->onMessage(this, buffer);
}

void Session::run(){
    this->_readyState = SessionLifeCycle::OPEN;
    LoopInside::run();
}

void Session::close(SessionCloseCode code){

    if(!(this->readyState == SessionLifeCycle::OPEN || this->readyState == SessionLifeCycle::CONNECTING)) return;

    this->_readyState = SessionLifeCycle::CLOSING;

    try{
        CloseReason c(code);
        this->socket->close(c);
    }
    catch(boost::system::system_error const& e){}

    this->_readyState = SessionLifeCycle::CLOSED;
}

void Session::send(IOBuffer &data) const {
    if(this->readyState != SessionLifeCycle::OPEN) return;
    this->socket->write(data.data());
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
