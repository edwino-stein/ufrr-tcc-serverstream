#if !defined  _AV_VIDEO_OUTPUT_HPP
    #define _AV_VIDEO_OUTPUT_HPP

    #include "av/CodecVideo.hpp"
    #include "av/FormatOutput.hpp"

    namespace av {

        class VideoOutput : public av::CodecVideo, public av::FormatOutput {

            public:

                VideoOutput();
                virtual ~VideoOutput();

                virtual void rescaleTs(AVPacket *packet) const override;
                virtual void close() override;

                virtual void encode(AVFrame *frame, EncodeListener *listener) const;
        };
    }

#endif