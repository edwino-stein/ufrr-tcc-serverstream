#include "av/FormatOutput.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"

using namespace av;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;

FormatOutput::FormatOutput() : Format(), format(_format){
    this->_format = NULL;
}

FormatOutput::~FormatOutput(){}