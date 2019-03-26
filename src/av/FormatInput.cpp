#include "av/FormatInput.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"

using namespace av;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;

FormatInput::FormatInput() : Format(), format(_format){
    this->_format = NULL;
}

FormatInput::~FormatInput(){}