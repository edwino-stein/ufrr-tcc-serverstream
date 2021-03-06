#include "ws/Session.hpp"
#include "ws/SessionListener.hpp"

using namespace ws;

using runtime::LoopInside;
using exceptions::WsErrorException;
using boost::system::system_error;
using boost::system::error_code;

Session::Session(WSocket &&socket, SessionListener * const listener) :
    LoopInside(), listener(listener), socket(std::move(socket)),
    readyState(_readyState), type(_type) {

    this->_readyState = SessionLifeCycle::CONNECTING;
    this->setType(SessionMessageType::TEXT);
}

Session::~Session(){
    if(this->readyState <= SessionLifeCycle::CLOSING) this->close();
    LoopInside::stop(true);
}

void Session::loop(){

    if(this->readyState != SessionLifeCycle::OPEN) return;

    IOBuffer buffer;

    try{
        if(this->socket.read(buffer) > 0) this->listener->onMessage(this, buffer);
    }
    catch(system_error const& se){

        if(se.code() == boost::asio::error::eof) return;

        if(se.code() == websocket::error::closed){
            this->listener->onClose(this, se.code().value());
            this->close();
            return;
        }

        this->listener->onError(this, WsErrorException(se));
    }
    catch(std::exception const& e){
        this->listener->onClose(this, CloseCodes::internal_error);
        this->close();
        return;
    }
}

void Session::run(){
    this->_readyState = SessionLifeCycle::OPEN;
    LoopInside::run();
}

void Session::stop(){
    LoopInside::stop(false);
}

void Session::close(){

    if(this->readyState == SessionLifeCycle::CLOSED) return;

    this->_readyState = SessionLifeCycle::CLOSING;

    boost::system::error_code ec;
    websocket::teardown(
        boost::beast::role_type::server,
        this->socket.next_layer(),
        ec
    );

    this->_readyState = SessionLifeCycle::CLOSED;
    this->stop();
}

void Session::close(CloseCodes code){

    if(this->readyState == SessionLifeCycle::CLOSED) return;

    this->_readyState = SessionLifeCycle::CLOSING;

    try{
        this->socket.close(code);
    }
    catch(system_error &e){}

    this->close();
}

void Session::send(IOBuffer &data){

    if(this->readyState != SessionLifeCycle::OPEN) return;

    try{
        this->socket.write(data.data());
    }
    catch(system_error const& e){
        this->listener->onError(this, WsErrorException(e));
    }
}

void Session::setType(SessionMessageType type){

    if(!(this->readyState == SessionLifeCycle::OPEN || this->readyState == SessionLifeCycle::CONNECTING)) return;

    switch(type){

        case SessionMessageType::BINARY:
            this->socket.text(false);
            this->socket.binary(true);
            this->_type = SessionMessageType::BINARY;
        break;

        default:
        case SessionMessageType::TEXT:
            this->socket.binary(false);
            this->socket.text(true);
            this->_type = SessionMessageType::TEXT;
        break;
    }
}
