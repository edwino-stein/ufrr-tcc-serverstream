#include "av/libav.hpp"

void av::initAll(){

    #if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
        av_register_all();
        avcodec_register_all();
    #endif

    avdevice_register_all();

    #if LIBAVFILTER_VERSION_INT < AV_VERSION_INT(7, 40, 101)
        avfilter_register_all();
    #endif
}

//For missing avio_context_free on old versions
#if LIBAVFORMAT_VERSION_INT <= AV_VERSION_INT(57, 56, 101)
void avio_context_free(AVIOContext **ps){
    av_freep(ps);
}
#endif
