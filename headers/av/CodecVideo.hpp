#if !defined  _AV_CODEC_VIDEO_HPP
    #define _AV_CODEC_VIDEO_HPP

    #include "av/Codec.hpp"

    namespace av {

        class CodecVideo : public av::Codec {

            protected:

                int _videoStreamIdx;
                AVStream *_videoStream;

            public:

                int const &videoStreamIdx;
                AVStream * const &videoStream;

                CodecVideo();
                virtual ~CodecVideo();
        };
    }

#endif