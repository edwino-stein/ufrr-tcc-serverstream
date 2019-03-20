#include "av/CodecVideo.hpp"

using namespace av;

CodecVideo::CodecVideo() : Codec(), videoStreamIdx(_videoStreamIdx), videoStream(_videoStream) {
    this->_videoStreamIdx = -1;
    this->_videoStream = NULL;
}

CodecVideo::~CodecVideo(){
    // TODO: verificar como liberar a memoria do jeito certo
    if(this->_videoStream != NULL){
        av_freep(&(this->_videoStream));
    }
}
