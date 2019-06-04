#include "av/VideoMemoryOutput.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using namespace av;
using namespace json;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;
using exceptions::InvalidJsonConfigException;

VideoMemoryOutput::VideoMemoryOutput(memoryWriteHandle callback) : VideoFileOutput("Video Memory Output"), avioCtx(_avioCtx) {
    this->callback = callback;
    this->avioBufferSize = 8192;
    this->avioBufferCtx = NULL;
    this->_avioCtx = NULL;
}

VideoMemoryOutput::~VideoMemoryOutput(){
    this->close();
}

void VideoMemoryOutput::init(){
    VideoFileOutput::init();
}

void VideoMemoryOutput::init(JsonObject cfg){

    if(cfg["buffersize"].is<int>())
        this->avioBufferSize = std::abs(cfg["buffersize"].as<int>());

    VideoFileOutput::init(cfg);
}

void VideoMemoryOutput::openFile(String &fileName, AVDictionary **options){

    if(this->formatCtx == NULL)
        throw AvErrorException(AVERROR_INVALIDDATA, Exception("Video format not initialized"));

    if(this->videoStream == NULL)
        throw AvErrorException(AVERROR_STREAM_NOT_FOUND, Exception("Video stream not initialized."));

    if(this->codecCtx == NULL)
        throw AvErrorException(AVERROR_FILTER_NOT_FOUND, Exception("Video codec not initialized."));

    int r;

    this->avioBufferCtx = (uint8_t *) av_malloc(this->avioBufferSize);
    this->_avioCtx = avio_alloc_context(
        this->avioBufferCtx,
        this->avioBufferSize,
        1,
        this,
        NULL,
        VideoMemoryOutput::avioWrite,
        NULL
    );

    if(this->_avioCtx == NULL)
        throw AvErrorException(AVERROR_INVALIDDATA , Exception("AVIO not initialized."));

    this->formatCtx->pb = this->avioCtx;

    r = avformat_write_header(this->formatCtx, NULL);
    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avformat_write_header", r));
}

void VideoMemoryOutput::closeFile(){

    if(this->formatCtx != NULL) this->formatCtx->pb = NULL;
    VideoOutput::closeFile();

    if(this->avioBufferCtx != NULL) av_freep(&(this->avioBufferCtx));
    if(this->_avioCtx != NULL) avio_context_free(&(this->_avioCtx));
}

int VideoMemoryOutput::avioWrite(void *opaque, uint8_t *buf, int buf_size){
    VideoMemoryOutput *o = (VideoMemoryOutput *) opaque;
    return o->callback(buf, buf_size, o);
}
