#include "av/FormatInput.hpp"

using namespace av;

FormatInput::FormatInput() : Format(), format(_format){
    this->_format = NULL;
}

FormatInput::~FormatInput(){
    // TODO: verificar como liberar a memoria do jeito certo
}

void FormatInput::read(AVPacket *packet) const {

    int r = av_read_frame(this->formatCtx, packet);

    if (r < 0){
        // TODO: exceptions
        throw r;
    }
}