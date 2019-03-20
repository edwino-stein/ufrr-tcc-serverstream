#include "av/VideoInput.hpp"
#include "av/DecodeListener.hpp"

using namespace av;

VideoInput::VideoInput() : CodecVideo(), FormatInput(){}
VideoInput::~VideoInput(){}

void VideoInput::rescaleTs(AVPacket *packet) const {
    av_packet_rescale_ts(
        packet,
        this->videoStream->time_base,
        this->codecCtx->time_base
    );
}

void VideoInput::close(){
    Codec::close();
    Format::close();
}

void VideoInput::decode(AVPacket *packet, DecodeListener *listener) const {

    int r;

    r = avcodec_send_packet(this->codecCtx, packet);
    if(r < 0) return;

    AVFrame *frame = av_frame_alloc();

    while(!r){
        r = avcodec_receive_frame(this->codecCtx, frame);
        if(!r) listener->onDecoded(frame);
    }

    av_frame_free(&frame);
}