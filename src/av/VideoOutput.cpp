#include "av/VideoOutput.hpp"

#include <cstring>

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"

using namespace av;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;

VideoOutput::VideoOutput() : CodecVideo(), FormatOutput(){}
VideoOutput::~VideoOutput(){}

bool VideoOutput::encode(AVFrame *const frame, const bool rescaleTs) const {

    int r;

    r = avcodec_send_frame(this->codecCtx, frame);
    if(r < 0){
        if(r == AVERROR(EAGAIN)) return false;
        throw AvErrorException(r, ReturnValueException("avcodec_send_frame", r));
    }

    AVPacket *packet = av_packet_alloc();

    while(true){

        r = avcodec_receive_packet(this->codecCtx, packet);
        if(r == AVERROR_EOF) break;

        if(r < 0){
            av_packet_free(&packet);
            if(r == AVERROR(EAGAIN)) return false;
            throw AvErrorException(r, ReturnValueException("avcodec_receive_packet", r));
        }

        if(rescaleTs){
            av_packet_rescale_ts(
                packet,
                this->codecCtx->time_base,
                this->videoStream->time_base
            );
        }

        r = av_interleaved_write_frame(this->formatCtx, packet);
        if(r < 0){
            av_packet_free(&packet);
            if(r == AVERROR(EAGAIN)) return false;
            throw AvErrorException(r, ReturnValueException("av_interleaved_write_frame", r));
        }
    }

    av_packet_free(&packet);
    return true;
}

void VideoOutput::openFile(String &fileName, AVDictionary **options){

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_INVALIDDATA, Exception("Video format not initialized"));

    if(this->videoStream == NULL)
        throw AvErrorException(AVERROR_STREAM_NOT_FOUND, Exception("Video stream not initialized."));

    if(this->codecCtx == NULL)
        throw AvErrorException(AVERROR_FILTER_NOT_FOUND, Exception("Video codec not initialized."));

    int r = 0;

    strcpy(this->formatCtx->filename, fileName.c_str());

    r = avio_open(
        &(this->formatCtx->pb),
        this->formatCtx->filename,
        AVIO_FLAG_WRITE
    );

    if(r < 0) throw AvErrorException(r, ReturnValueException("avio_open", r));

    r = avformat_write_header(this->formatCtx, NULL);
    if(r < 0) throw AvErrorException(r, ReturnValueException("avformat_write_header", r));
}

void VideoOutput::initFormat(String demuxer, String codec){

    this->_format = av_guess_format(
        demuxer.c_str(),
        NULL,
        NULL
    );

    if(this->format == NULL)
        throw AvErrorException(AVERROR_DEMUXER_NOT_FOUND, ReturnValueException("av_guess_format", "NULL"));

    this->_format->video_codec = Codec::getCodecIdByName(codec);
    if(this->_format->video_codec == AV_CODEC_ID_NONE)
        throw AvErrorException(AVERROR_ENCODER_NOT_FOUND);

    this->_formatCtx = avformat_alloc_context();
    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_DEMUXER_NOT_FOUND, ReturnValueException("avformat_alloc_context", "NULL"));

    this->formatCtx->oformat = this->format;
}

void VideoOutput::initVideoStream(){

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_STREAM_NOT_FOUND, Exception("Video format not initialized."));

    this->_videoStream = avformat_new_stream(this->formatCtx, NULL);
    if(this->videoStream == NULL)
        throw AvErrorException(AVERROR_STREAM_NOT_FOUND, ReturnValueException("avformat_new_stream", "NULL"));
}

void VideoOutput::initCodec(AFunction<void(AVCodecContext *, AVCodec *)> setupCodec){

    if(this->videoStream == NULL)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, AvErrorException(AVERROR_STREAM_NOT_FOUND));

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_DECODER_NOT_FOUND, Exception("Video format not initialized"));

    int r = 0;

    this->_codec = avcodec_find_encoder(this->format->video_codec);
    if(this->codec == NULL)
        throw AvErrorException(AVERROR_ENCODER_NOT_FOUND, ReturnValueException("avcodec_find_encoder", "NULL"));

    this->_codecCtx = avcodec_alloc_context3(this->codec);
    if(this->codecCtx == NULL)
        throw AvErrorException(AVERROR_ENCODER_NOT_FOUND, ReturnValueException("avcodec_alloc_context3", "NULL"));

    this->codecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    this->codecCtx->codec_id = this->format->video_codec;

    if(this->formatCtx->oformat->flags & AVFMT_GLOBALHEADER){
        this->codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    r = avcodec_parameters_from_context(this->videoStream->codecpar, this->codecCtx);
    if (r < 0)
        throw AvErrorException(r, ReturnValueException("avcodec_parameters_from_context", r));

    setupCodec(this->codecCtx, this->codec);

    if(this->codecCtx->pix_fmt == AV_PIX_FMT_NONE){
        this->codecCtx->pix_fmt = this->codec->pix_fmts != NULL ? this->codec->pix_fmts[0] : AV_PIX_FMT_NONE;
    }

    this->videoStream->time_base = this->codecCtx->time_base;

    r = avcodec_open2(this->codecCtx, this->codec, NULL);
    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avcodec_open2", r));
}

void VideoOutput::initCodec(){
    this->initCodec([](AVCodecContext *ctx, AVCodec *c){ return; });
}

void VideoOutput::closeFile(){
    if(this->formatCtx != NULL){
        if(this->formatCtx->pb != NULL) avio_closep(&(this->formatCtx->pb));
    }
}

void VideoOutput::closeFormat(){
    if(this->formatCtx != NULL){
        av_write_trailer(this->formatCtx);
        avformat_free_context(this->formatCtx);
        this->_formatCtx = NULL;
        this->_format = NULL;
    }
}

void VideoOutput::closeCodec(){
    if(this->codecCtx != NULL){
        avcodec_close(this->codecCtx);
        avcodec_free_context(&(this->_codecCtx));
        this->_codec = NULL;
    }
}
