#include "ws/ServerCallback.hpp"
#include "ws/SessionListener.hpp"
#include "ws/ServerListener.hpp"
#include "ws/Session.hpp"

#include "boost/system/error_code.hpp"

using namespace ws;
using namespace boost::system::errc;
using exceptions::WsErrorException;

ServerCallback::ServerCallback(ServerListener &listener) : listener(listener){}
ServerCallback::~ServerCallback(){}

void ServerCallback::onMessage(Session &session, IOBuffer &data){
    this->listener.onMessage(session, data);
}

void ServerCallback::onClose(Session &session, const int code){
    this->listener.onClose(session, code);
}

void ServerCallback::onError(Session &session, WsErrorException e){
    this->listener.onError(session, e);

    switch (e.getWsException().code().value()){
        case errc_t::bad_file_descriptor:
        case errc_t::operation_canceled:
        case errc_t::broken_pipe:
            this->onClose(session, CloseCodes::internal_error);
            session.close(CloseCodes::internal_error);
        break;
    }
}
