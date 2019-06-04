#include "av/FilterGraph.hpp"
#include "av/VideoInput.hpp"
#include "av/VideoOutput.hpp"
#include "av/Codec.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"

#include <sstream>

using namespace av;

using exceptions::Exception;
using exceptions::AvErrorException;
using exceptions::ReturnValueException;

FilterGraph::FilterGraph(){
    this->src = NULL;
    this->srcCtx = NULL;
    this->sink = NULL;
    this->sinkCtx = NULL;
    this->fg = NULL;
    this->inputs = NULL;
    this->outputs = NULL;
}

FilterGraph::~FilterGraph(){
    this->flush();
    if(this->fg != NULL) avfilter_graph_free(&(this->fg));
    if(this->inputs != NULL) avfilter_inout_free(&inputs);
    if(this->outputs != NULL) avfilter_inout_free(&outputs);
}

void FilterGraph::flush(VideoOutput * const out) const {

    if(!this->push(NULL)) return;

    AVFrame *frame = av_frame_alloc();
    while(this->pop(frame)) if(out != NULL) out->encode(frame, true);
    av_frame_free(&frame);
}

void FilterGraph::init(VideoInput * const in, VideoOutput * const out){

    int r;

    this->fg = avfilter_graph_alloc();
    this->src  = avfilter_get_by_name("buffer");
    this->sink = avfilter_get_by_name("buffersink");

    String inputArgs = this->parseInputArgs(in);

    r = avfilter_graph_create_filter(
        &(this->srcCtx),
        this->src,
        "in",
        inputArgs.c_str(),
        NULL,
        this->fg
    );

    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avfilter_graph_create_filter", r));

    AVBufferSinkParams *bufferSinkParams = av_buffersink_params_alloc();
    bufferSinkParams->pixel_fmts = out->codec->pix_fmts;

    r = avfilter_graph_create_filter(
        &(this->sinkCtx),
        this->sink,
        "out",
        NULL,
        bufferSinkParams,
        this->fg
    );

    av_free(bufferSinkParams);

    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avfilter_graph_create_filter", r));

    this->outputs = avfilter_inout_alloc();
    this->outputs->name = av_strdup("in");
    this->outputs->filter_ctx = this->srcCtx;
    this->outputs->pad_idx = 0;
    this->outputs->next = NULL;

    this->inputs = avfilter_inout_alloc();
    this->inputs->name = av_strdup("out");
    this->inputs->filter_ctx = this->sinkCtx;
    this->inputs->pad_idx = 0;
    this->inputs->next = NULL;

    String outputArgs = this->parseOutputArgs(out);

    r = avfilter_graph_parse_ptr(
        this->fg,
        outputArgs.c_str(),
        &(inputs),
        &(outputs),
        NULL
    );

    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avfilter_graph_parse_ptr", r));

    r = avfilter_graph_config(this->fg, NULL);
    if(r < 0)
        throw AvErrorException(r, ReturnValueException("avfilter_graph_config", r));
}

bool FilterGraph::push(AVFrame * const frame) const {
    return av_buffersrc_add_frame(this->srcCtx, frame) >= 0;
}

bool FilterGraph::pop(AVFrame * frame) const {
    return av_buffersink_get_frame(this->sinkCtx, frame) >= 0;
}

String FilterGraph::parseInputArgs(VideoInput * const in) const {
    std::ostringstream args;

    args << "video_size=" << std::to_string(in->codecCtx->width) << 'x' << std::to_string(in->codecCtx->height) << ':';
    args << "pix_fmt=" << std::to_string(in->codecCtx->pix_fmt) << ":";
    args << "time_base=" << std::to_string(in->codecCtx->time_base.num) << '/' << std::to_string(in->codecCtx->time_base.den) << ':';
    args << "pixel_aspect=" << std::to_string(in->codecCtx->sample_aspect_ratio.num) << '/' << std::to_string(in->codecCtx->sample_aspect_ratio.den);

    return args.str();
}

String FilterGraph::parseOutputArgs(VideoOutput * const out) const {
    std::ostringstream args;
    args << "scale='w=-1:h=" << out->codecCtx->height << "',";
    args << "format='" << Codec::getPixFmtName(out->codecCtx->pix_fmt) << "'";
    return args.str();
}
