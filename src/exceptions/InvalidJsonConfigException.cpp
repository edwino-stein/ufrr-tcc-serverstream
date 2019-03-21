#include "exceptions/InvalidJsonConfigException.hpp"

using namespace exceptions;

InvalidJsonConfigException::InvalidJsonConfigException(String key, String expected, String butType) : Exception(){
    this->init(
        "Invalid or missing JSON key \"" + key + "\": expected \"" + expected + "\", but found \"" + butType + "\"",
        NULL,
        2,
        2
    );
}

InvalidJsonConfigException::InvalidJsonConfigException(String key, String expected) : Exception(){
    this->init(
        "Invalid or missing JSON key \"" + key + "\": expected \"" + expected + "\"",
        NULL,
        2,
        2
    );
}

InvalidJsonConfigException::~InvalidJsonConfigException(){}