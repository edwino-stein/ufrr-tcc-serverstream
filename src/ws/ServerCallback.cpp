#include "ws/ServerCallback.hpp"
#include "ws/SessionListener.hpp"
#include "ws/ServerListener.hpp"
#include "ws/Session.hpp"

using namespace ws;
using exceptions::Exception;

ServerCallback::ServerCallback(ServerListener * const listener) : listener(listener){}
ServerCallback::~ServerCallback(){}

void ServerCallback::onMessage(Session * const session, IOBuffer &data){
    if(this->listener != NULL) this->listener->onMessage(session, data);
}

void ServerCallback::onClose(Session * const session, const int code){
    if(this->listener != NULL) this->listener->onClose(session, code);
    session->close(SessionCloseCode::normal);
}

void ServerCallback::onError(Session * const session, Exception e){
    if(this->listener != NULL) this->listener->onError(session, e);
}
