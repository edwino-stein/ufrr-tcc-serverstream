#include "exceptions/ReturnValueException.hpp"

using namespace exceptions;

ReturnValueException::ReturnValueException(String functionName, const int rcode) : Exception(){
    this->init(
        "The function \"" + functionName +"\" returns error code " + std::to_string(rcode),
        NULL,
        2,
        2
    );
}

ReturnValueException::ReturnValueException(String functionName, String rcustom){
    this->init(
        "The function \"" + functionName +"\" returns error code " + rcustom,
        NULL,
        2,
        2
    );
}

ReturnValueException::~ReturnValueException(){}