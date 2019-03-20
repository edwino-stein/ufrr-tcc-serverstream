#include "av/Format.hpp"

using namespace av;

Format::Format() : formatCtx(_formatCtx){
    this->_formatCtx = NULL;
}

Format::~Format(){
    // TODO: verificar como liberar a memoria do jeito certo
    this->close();
}

void Format::close(){

    if(this->_formatCtx != NULL){

        if(this->_formatCtx->pb != NULL) avio_close(this->_formatCtx->pb);
        
        av_free(this->_formatCtx);
        this->_formatCtx = NULL;
    }
}