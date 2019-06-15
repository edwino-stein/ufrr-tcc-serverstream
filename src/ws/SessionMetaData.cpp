#include "ws/SessionMetaData.hpp"
#include <sstream>

using namespace ws;
using namespace net;

SessionMetaData::SessionMetaData(ws::WSocket &socket, HTTPRequest &request) :
    userAgent(_userAgent), secWsKey(_secWsKey), clientIp(_clientIp){

    this->_secWsKey = request[http::field::sec_websocket_key].to_string();
    this->_userAgent = request[http::field::user_agent].to_string();
    this->_clientIp = socket.next_layer().remote_endpoint().address().to_string();
}

SessionMetaData::SessionMetaData(const SessionMetaData& md) :
    userAgent(_userAgent), secWsKey(_secWsKey), clientIp(_clientIp){

    this->_secWsKey = md.secWsKey;
    this->_userAgent = md.userAgent;
    this->_clientIp = md.clientIp;
}

SessionMetaData::~SessionMetaData(){}

String SessionMetaData::toString() const {

    std::ostringstream oss;

    oss << "Client \"" << this->_secWsKey << "\"";
    oss << " from " << this->_clientIp;
    oss << " [" << this->_userAgent << "]";

    return oss.str();
}
