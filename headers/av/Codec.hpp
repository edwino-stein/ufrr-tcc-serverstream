#if !defined  _AV_CODEC_HPP
    #define _AV_CODEC_HPP

    #include "av/libav.hpp"

    namespace av {

        class Codec {

            protected:

                AVCodec *_codec;
                AVCodecContext *_codecCtx;

            public:

                AVCodec * const &codec;
                AVCodecContext * const &codecCtx;

                Codec();
                virtual ~Codec();

                virtual void init(json::JsonObject cfg)=0;
                virtual void close();

                virtual void rescaleTs(AVPacket *packet) const =0;

                static enum AVCodecID getCodecIdByName(String codecName);
        };
    }

#endif