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

void FormatOutput::write(AVPacket *packet) const {

    int r = av_interleaved_write_frame(this->formatCtx, packet);

    if(r < 0)
        if (r < 0)
        throw AvErrorException(r, ReturnValueException("av_interleaved_write_frame", r));
}