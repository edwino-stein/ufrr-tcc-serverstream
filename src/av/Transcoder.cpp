#include "av/Transcoder.hpp"

#include "av/VideoInput.hpp"
#include "av/VideoOutput.hpp"
#include "av/FilterGraph.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"

using namespace av;

using runtime::TaskContext;
using runtime::LoopTask;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;

Transcoder::Transcoder(VideoInput *const decoder, VideoOutput * const encoder) : decoder(decoder), encoder(encoder), readError(_readError){
    this->filterGraph = NULL;
    this->decoderTask = NULL;
    this->encoderTask = NULL;
    this->_readError = true;
    this->decodeRescaleTs = false;
    this->encodeRescaleTs = true;
}

Transcoder::~Transcoder(){

    this->_readError = true;

    if(this->filterGraph != NULL){
        this->filterGraph->flush();
        delete this->filterGraph;
    }

    if(this->decoderTask != NULL){
        this->decoderTask->stop(true);
        delete this->decoderTask;
    }

    if(this->encoderTask != NULL){
        this->encoderTask->stop(true);
        delete this->encoderTask;
    }
}

void Transcoder::onDecoded(AVFrame *frame){

    frame->pts = frame->best_effort_timestamp;

    this->filterGraphMtx.lock();
    this->filterGraph->push(frame);
    this->filterGraphMtx.unlock();
}

void Transcoder::init(json::JsonObject cfg){

    this->decoder->init(cfg["decoder"].as<json::JsonObject>());
    if(cfg["decoder"]["rescaleTs"].is<bool>())
        this->decodeRescaleTs = cfg["decoder"]["rescaleTs"].as<bool>();

    this->encoder->init(cfg["encoder"].as<json::JsonObject>());
    if(cfg["encoder"]["rescaleTs"].is<bool>())
        this->encodeRescaleTs = cfg["encoder"]["rescaleTs"].as<bool>();

    this->filterGraph = new FilterGraph();
    this->filterGraph->init(this->decoder, this->encoder);

    this->decoderTask = new LoopTask(this, [](TaskContext * const ctx){

        Transcoder * const me = (Transcoder * const) ctx;
        if(me->readError) return;

        try{
            if(me->decoder->read(me->decodeRescaleTs)) me->decoder->decode(me);
            else me->_readError = true;
        }
        catch(Exception &e){
            me->_readError = true;
        }
    });

    this->encoderTask = new LoopTask(this, [](TaskContext * const ctx){

        Transcoder * const me = (Transcoder * const) ctx;

        AVFrame *frame = av_frame_alloc();
        bool hasFrame = true;

        while(hasFrame){

            me->filterGraphMtx.lock();
            hasFrame = me->filterGraph->pop(frame);
            me->filterGraphMtx.unlock();

            if(hasFrame) me->encoder->encode(frame, me->encodeRescaleTs);
        }

        av_frame_free(&frame);
    });
}

void Transcoder::run(){
    this->_readError = false;
    if(this->decoderTask != NULL) this->decoderTask->run();
    if(this->encoderTask != NULL) this->encoderTask->run();
}

void Transcoder::stop(){
    if(this->decoderTask != NULL) this->decoderTask->stop(true);
    if(this->encoderTask != NULL) this->encoderTask->stop(true);
}
