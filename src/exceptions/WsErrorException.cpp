#include "exceptions/WsErrorException.hpp"
#include <sstream>

using namespace exceptions;
using boost::system::system_error;

WsErrorException::WsErrorException(const system_error wsException) : Exception(), wsException(wsException) {
    this->init(
        this->getWsError(wsException),
        NULL,
        2,
        2
    );
}

WsErrorException::~WsErrorException(){}

const system_error &WsErrorException::getWsException() const {
    return this->wsException;
}

String WsErrorException::getWsError(const system_error &wsException) const {
    std::ostringstream oss;
    oss << "Websocket error: ";
    oss << wsException.what();
    return oss.str();
}
