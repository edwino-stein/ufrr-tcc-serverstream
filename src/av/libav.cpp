#include "av/libav.hpp"

void av::initAll(){
    av_register_all(); 
    avcodec_register_all();
    avdevice_register_all();
    avfilter_register_all();
}