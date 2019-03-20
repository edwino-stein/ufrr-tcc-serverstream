#include "av/Codec.hpp"

using namespace av;

Codec::Codec() : codec(_codec), codecCtx(_codecCtx) {
    this->_codec = NULL;
    this->_codecCtx = NULL;
}

Codec::~Codec(){
    // TODO: verificar como liberar a memoria do jeito certo
    this->close();
}

void Codec::close(){
    if(this->_codecCtx != NULL){
        avcodec_close(this->_codecCtx);
        this->_codecCtx = NULL;
    }
}

enum AVCodecID Codec::getCodecIdByName(String codecName){
    const AVCodecDescriptor* avcd = avcodec_descriptor_get_by_name(codecName.c_str());
    return avcd != NULL ? avcd->id : AV_CODEC_ID_NONE;
}