#include "av/VideoInput.hpp"
#include "av/DecodeListener.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"

using namespace av;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;

VideoInput::VideoInput() : CodecVideo(), FormatInput(){}
VideoInput::~VideoInput(){}

void VideoInput::rescaleTs(AVPacket *packet) const {
    av_packet_rescale_ts(
        packet,
        this->videoStream->time_base,
        this->codecCtx->time_base
    );
}

bool VideoInput::push(AVPacket *packet) const {
    int r;

    r = avcodec_send_packet(this->codecCtx, packet);

    if(r < 0){
        if(r == AVERROR(EAGAIN)) return false;
        throw AvErrorException(r, ReturnValueException("avcodec_send_packet", r));
    }

    return true;
}

bool VideoInput::read(AVPacket *packet, const bool rescaleTs, const bool push) const {

    int r;

    r = av_read_frame(this->formatCtx, packet);

    if(r < 0){
        if(r == AVERROR(EAGAIN)) return false;
        throw AvErrorException(r, ReturnValueException("av_read_frame", r));
    }

    if(packet->stream_index != this->videoStreamIdx) return true;

    if(rescaleTs){
        av_packet_rescale_ts(
            packet,
            this->videoStream->time_base,
            this->codecCtx->time_base
        );
    }

    if(push){
        r = avcodec_send_packet(this->codecCtx, packet);
        if(r < 0){
            if(r == AVERROR(EAGAIN)) return false;
            throw AvErrorException(r, ReturnValueException("avcodec_send_packet", r));
        }
    }

    return true;
}

bool VideoInput::decode(DecodeListener *const listener) const {

    int r;
    AVFrame *frame = av_frame_alloc();

    while(true){

        r = avcodec_receive_frame(this->codecCtx, frame);

        if(r == 0){
            listener->onDecoded(frame);
            continue;
        }

        av_frame_free(&frame);

        if(r == AVERROR(EAGAIN) || r == AVERROR_EOF) break;
        throw AvErrorException(r, ReturnValueException("avcodec_receive_frame", r));
    }

    return true;
}

void VideoInput::openFile(String &filename, AVDictionary **options){

    int r = 0;

    r = avformat_open_input(
        &(this->_formatCtx),
        filename.c_str(),
        this->format,
        options
    );

    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avformat_open_input", r));
    
    this->initFormat("", "");
}

void VideoInput::initFormat(String demuxer, String codec){

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_DEMUXER_NOT_FOUND, Exception("Video format not initialized."));

    int r = 0;
    r = avformat_find_stream_info(this->formatCtx, NULL);
    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avformat_open_input", r));

    this->_format = this->formatCtx->iformat;
}

void VideoInput::initVideoStream(){

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_STREAM_NOT_FOUND, Exception("Video format not initialized."));
   
    int r = 0;

    r = av_find_best_stream(
        this->formatCtx,
        AVMEDIA_TYPE_VIDEO,
        -1,
        -1,
        NULL,
        0
    );

    if(r < 0) throw AvErrorException(r, ReturnValueException("av_find_best_stream", r));

    this->_videoStream = this->formatCtx->streams[r];
    this->_videoStreamIdx = r;
}

void VideoInput::initCodec(){

    if(this->videoStream == NULL)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, AvErrorException(AVERROR_STREAM_NOT_FOUND));

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, Exception("Video format not initialized"));
    
    int r = 0;

    this->_codec = avcodec_find_decoder(this->videoStream->codecpar->codec_id);
    if(this->codec == NULL)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, ReturnValueException("avcodec_find_decoder", "NULL"));

    this->_codecCtx = avcodec_alloc_context3(this->codec);
    if(this->codecCtx == NULL)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, ReturnValueException("avcodec_alloc_context3", "NULL"));

    r = avcodec_parameters_to_context(this->codecCtx, this->videoStream->codecpar);
    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avcodec_parameters_to_context", r));

    this->codecCtx->framerate = av_guess_frame_rate(this->formatCtx, this->videoStream, NULL);
    if(this->codecCtx->framerate.num == 0 && this->codecCtx->framerate.den == 1)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, Exception("Invalid framerate"));

    r = avcodec_open2(this->codecCtx, this->codec, NULL);
    if (r < 0)
        throw AvErrorException(r, ReturnValueException("avcodec_parameters_to_context", r));
}

void VideoInput::initCodec(AFunction<void(AVCodecContext *, AVCodec *)> setupCodec){
    this->initCodec();
}

void VideoInput::closeFile(){
    this->closeFormat();
}

void VideoInput::closeFormat(){
    if(this->formatCtx != NULL){
        avformat_close_input(&(this->_formatCtx));
        this->_format = NULL;
    }
}

void VideoInput::closeCodec(){
    if(this->codecCtx != NULL){
        avcodec_close(this->codecCtx);
        avcodec_free_context(&(this->_codecCtx));
        this->_codec = NULL;
    }
}