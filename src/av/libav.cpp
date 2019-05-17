#include "av/libav.hpp"

void av::initAll(){
    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
    avfilter_register_all();
}

//For missing avio_context_free on old versions
#if LIBAVFORMAT_VERSION_INT <= AV_VERSION_INT(57, 56, 101)
void avio_context_free(AVIOContext **ps){
    av_freep(ps);
}
#endif
