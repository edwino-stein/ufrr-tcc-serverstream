#if !defined  _AV_FILTER_GRAPH_HPP
    #define _AV_FILTER_GRAPH_HPP

    #include "av/libav.hpp"

    namespace av {

        class FilterGraph {

            protected:

                const AVFilter *src;
                AVFilterContext *srcCtx;

                const AVFilter *sink;
                AVFilterContext *sinkCtx;

                AVFilterGraph *fg;

                AVFilterInOut *inputs;
                AVFilterInOut *outputs;

                String parseInputArgs(VideoInput * const in) const;
                String parseOutputArgs(VideoOutput * const out) const;

            public:

                FilterGraph();
                virtual ~FilterGraph();

                virtual void init(VideoInput * const in, VideoOutput * const out);
                virtual bool push(AVFrame * const frame) const;
                virtual bool pop(AVFrame * frame) const;
                virtual void flush(VideoOutput * const out = NULL) const;
        };
    }

#endif
