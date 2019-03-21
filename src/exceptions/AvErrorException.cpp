#include "exceptions/AvErrorException.hpp"
#include "av/libav.hpp"

using namespace exceptions;

AvErrorException::AvErrorException(const int errnum) : Exception() {
    this->init(
        this->getAvError(errnum),
        NULL,
        2,
        2
    );
}

AvErrorException::AvErrorException(const int errnum, const Exception &prev) : Exception() {
    this->init(
        this->getAvError(errnum),
        new Exception(prev),
        2,
        2
    );    
}

AvErrorException::~AvErrorException(){}

String AvErrorException::getAvError(const int errnum) const {

    char errbuf[255];
    String error = "Unknown error occurred";

    if(av_strerror(errnum, errbuf, sizeof(errbuf)) == 0){
        error = errbuf;
    }

    return "AV error: " + error + " (" + std::to_string(errnum) + ')';
}