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

void FormatInput::read(AVPacket *packet) const {

    int r = av_read_frame(this->formatCtx, packet);

    if(r < 0)
        throw AvErrorException(r, ReturnValueException("av_read_frame", r));
}