#if !defined  _AV_TRANSCODER_HPP
    #define _AV_TRANSCODER_HPP

    #include "av/libav.hpp"
    #include "av/DecodeListener.hpp"
    #include "runtime/LoopTask.hpp"

    namespace av {

        class Transcoder : public av::DecodeListener,  public runtime::TaskContext{

            protected:

                av::VideoInput * const decoder;
                runtime::LoopTask *decoderTask;
                bool decodeRescaleTs;

                bool _readError;

                av::VideoOutput * const encoder;
                runtime::LoopTask *encoderTask;
                bool encodeRescaleTs;

                Mutex filterGraphMtx;
                av::FilterGraph *filterGraph;

            public:

                bool const &readError;

                Transcoder(av::VideoInput *const decoder, av::VideoOutput * const encoder);
                virtual ~Transcoder();

                virtual void init(json::JsonObject cfg);
                virtual void run();
                virtual void stop();

                virtual void onDecoded(AVFrame *frame) override;
        };
    }

#endif
