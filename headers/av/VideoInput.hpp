#if !defined  _AV_VIDEO_INPUT_HPP
    #define _AV_VIDEO_INPUT_HPP

    #include "av/CodecVideo.hpp"
    #include "av/FormatInput.hpp"

    namespace av {

        class VideoInput : public av::CodecVideo, public av::FormatInput {

            public:

                VideoInput();
                virtual ~VideoInput();

                virtual void rescaleTs(AVPacket *packet) const override;
                virtual void close() override;

                virtual void decode(AVPacket *packet, DecodeListener *listener) const;
        };
    }

#endif