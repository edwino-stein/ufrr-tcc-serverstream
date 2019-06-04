#include "av/VideoFileOutput.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using namespace av;
using namespace json;
using exceptions::Exception;
using exceptions::InvalidJsonConfigException;

VideoFileOutput::VideoFileOutput(String fileName) : VideoOutput(){
    this->fileName = fileName;
}

VideoFileOutput::~VideoFileOutput(){
    this->close();
}

void VideoFileOutput::init(){
    DynamicJsonDocument doc(1024);

    doc["demuxer"] = "mpegts";
    doc["codec"] = "mpeg1video";
    doc["width"] = 640;
    doc["height"] = 480;
    doc["bitrate"] = 1000000;
    doc["framerate"] = 30;
    doc["pixfmt"] = "yuv420p";

    this->init(doc.as<JsonObject>());
}

void VideoFileOutput::init(JsonObject cfg){

    if(!cfg["demuxer"].is<const char *>())
        throw InvalidJsonConfigException("demuxer", "string");

    if(!cfg["codec"].is<const char *>())
        throw InvalidJsonConfigException("codec", "string");

    if(!cfg["width"].is<int>())
        throw InvalidJsonConfigException("width", "int");

    if(!cfg["height"].is<int>())
        throw InvalidJsonConfigException("height", "int");

    if(!cfg["bitrate"].is<int>())
        throw InvalidJsonConfigException("bitrate", "int");

    if(!cfg["framerate"].is<int>())
        throw InvalidJsonConfigException("framerate", "int");

    enum AVPixelFormat pixFmt = AV_PIX_FMT_NONE;

    if(cfg["pixfmt"].is<const char *>()){
        enum AVPixelFormat pixFmt = Codec::getPixFmtByName(cfg["pixfmt"].as<String>());
        if(!Codec::supportsPixFmt(cfg["codec"].as<String>(), pixFmt))
            throw Exception("Incompatible pixel format with defined codec");
    }

    this->initFormat(cfg["demuxer"].as<String>(), cfg["codec"].as<String>());
    this->initVideoStream();

    this->initCodec([&cfg, &pixFmt](AVCodecContext *codecCtx, AVCodec *codec){

        codecCtx->width = std::abs(cfg["width"].as<int>());
        codecCtx->height = std::abs(cfg["height"].as<int>());
        codecCtx->bit_rate = std::abs(cfg["bitrate"].as<int>());
        codecCtx->time_base = (AVRational){1, std::abs(cfg["framerate"].as<int>())};
        codecCtx->framerate = (AVRational){std::abs(cfg["framerate"].as<int>()), 1};

        codecCtx->pix_fmt = pixFmt;

        if(cfg["bframes"].is<int>())
            codecCtx->max_b_frames = std::abs(cfg["bframes"].as<int>());

        if(cfg["gop"].is<int>())
            codecCtx->gop_size = std::abs(cfg["gop"].as<int>());

        if(cfg["qblur"].is<float>())
            codecCtx->qblur = std::abs(cfg["qblur"].as<float>());

        if(cfg["qmax"].is<int>())
            codecCtx->qmax = std::abs(cfg["qmax"].as<int>());

        if(cfg["qmin"].is<int>())
            codecCtx->qmin = std::abs(cfg["qmin"].as<int>());

        if(cfg["compressionLevel"].is<int>())
            codecCtx->compression_level = std::abs(cfg["compressionLevel"].as<int>());
    });

    this->openFile(this->fileName);
}

void VideoFileOutput::close(){
    this->closeCodec();
    this->closeFile();
    this->closeFormat();
}

void VideoFileOutput::dump() const {
    Format::dump(this->fileName, true);
}
