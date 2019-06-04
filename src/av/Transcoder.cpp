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
    this->skipFrames = 0;
    this->skippedFrames = 0;
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

    int decoderFR = this->decoder->codecCtx->time_base.den;
    int encoderFR = this->encoder->codecCtx->time_base.den;
    this->skipFrames = (unsigned short) (decoderFR/encoderFR);

    this->decoderTask = new LoopTask(this, [](TaskContext * const ctx){

        Transcoder * const me = (Transcoder * const) ctx;
        if(me->readError) return;

        AVPacket *packet = av_packet_alloc();

        try{
            if(me->decoder->read(me->decodeRescaleTs, packet)){
                if(me->skippedFrames == 0){
                    me->decoder->push(packet);
                    me->decoder->decode(me);
                }
            }
            else me->_readError = true;

            me->skippedFrames++;
            if(me->skippedFrames >= me->skipFrames){
                me->skippedFrames = 0;
            }
        }
        catch(Exception &e){
            me->_readError = true;
        }

        av_packet_free(&packet);
    });

    this->encoderTask = new LoopTask(this, [](TaskContext * const ctx){

        Transcoder * const me = (Transcoder * const) ctx;

        AVFrame *frame = av_frame_alloc();
        bool hasFrame = true;

        while(hasFrame){

            me->filterGraphMtx.lock();
            hasFrame = me->filterGraph->pop(frame);
            me->filterGraphMtx.unlock();
    	    try{
               	if(hasFrame){
                    me->encoder->encode(frame, me->encodeRescaleTs);
                    av_frame_unref(frame);
                }
    	    }
            catch(Exception e){}
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
