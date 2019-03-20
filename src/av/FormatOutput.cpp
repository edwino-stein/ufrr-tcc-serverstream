#include "av/FormatOutput.hpp"

using namespace av;

FormatOutput::FormatOutput() : Format(), format(_format){
    this->_format = NULL;
}

FormatOutput::~FormatOutput(){
    // TODO: verificar como liberar a memoria do jeito certo
}

void FormatOutput::write(AVPacket *packet) const {

    int r = av_interleaved_write_frame(this->formatCtx, packet);

    if (r < 0){
        // TODO: exceptions
        throw r;
    }
}