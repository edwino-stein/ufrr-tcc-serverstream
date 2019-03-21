#include "av/Codec.hpp"

#include <algorithm>

using namespace av;

Codec::Codec() : codec(_codec), codecCtx(_codecCtx) {
    this->_codec = NULL;
    this->_codecCtx = NULL;
}

Codec::~Codec(){}

enum AVCodecID Codec::getCodecIdByName(String codecName){
    const AVCodecDescriptor* avcd = avcodec_descriptor_get_by_name(codecName.c_str());
    return avcd != NULL ? avcd->id : AV_CODEC_ID_NONE;
}

enum AVPixelFormat Codec::getPixFmtByName(String pixFmtName){
    return av_get_pix_fmt(pixFmtName.c_str());
}

bool Codec::supportsPixFmt(String codecName, enum AVPixelFormat pixFmt){

    if(pixFmt == AV_PIX_FMT_NONE) return false;

    AVCodec *codec = avcodec_find_encoder_by_name(codecName.c_str());
    if(codec == NULL) return false;
    if(codec->pix_fmts == NULL) return false;

    size_t size = sizeof(codec->pix_fmts)/sizeof(enum AVPixelFormat);
    for(size_t i = 0; i < size; ++i){
        if(codec->pix_fmts[i] == pixFmt) return true;
    }

    return false;
}