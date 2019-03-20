#include "av/VideoOutput.hpp"
#include "av/EncodeListener.hpp"

using namespace av;

VideoOutput::VideoOutput() : CodecVideo(), FormatOutput(){}
VideoOutput::~VideoOutput(){}

void VideoOutput::rescaleTs(AVPacket *packet) const {
    av_packet_rescale_ts(
        packet,
        this->codecCtx->time_base,
        this->videoStream->time_base
    );
}

void VideoOutput::close(){
    Codec::close();
    Format::close();
}

void VideoOutput::encode(AVFrame *frame, EncodeListener *listener) const {

    int r;

    r = avcodec_send_frame(this->codecCtx, frame);
    if(r < 0) return;

    AVPacket *packet = av_packet_alloc();

    while(!r){
        r = avcodec_receive_packet(this->codecCtx, packet);
        if(!r) listener->onEncoded(packet);
    }

    av_packet_free(&packet);
}