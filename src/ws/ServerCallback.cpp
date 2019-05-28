#include "ws/ServerCallback.hpp"
#include "ws/SessionListener.hpp"
#include "ws/ServerListener.hpp"
#include "ws/Session.hpp"

#include "boost/system/error_code.hpp"

using namespace ws;
using namespace boost::system::errc;
using exceptions::WsErrorException;

ServerCallback::ServerCallback(ServerListener * const listener) : listener(listener){}
ServerCallback::~ServerCallback(){}

void ServerCallback::onMessage(Session * const session, IOBuffer &data){
    if(this->listener != NULL) this->listener->onMessage(session, data);
}

void ServerCallback::onClose(Session * const session, const int code){
    if(this->listener != NULL) this->listener->onClose(session, code);
}

void ServerCallback::onError(Session * const session, WsErrorException e){
    if(this->listener != NULL) this->listener->onError(session, e);

    switch (e.getWsException().code().value()){
        case errc_t::bad_file_descriptor:
        case errc_t::operation_canceled:
        case errc_t::broken_pipe:
            this->onClose(session, CloseCodes::internal_error);
            session->close(CloseCodes::internal_error);
        break;
    }
}
