#if !defined  _AV_CODEC_HPP
    #define _AV_CODEC_HPP

    #include "av/libav.hpp"

    namespace av {

        class Codec {

            protected:

                AVCodec *_codec;
                AVCodecContext *_codecCtx;

                virtual void initCodec()=0;
                virtual void initCodec(AFunction<void(AVCodecContext *, AVCodec *)> setupCodec)=0;
                virtual void closeCodec()=0;

            public:

                AVCodec * const &codec;
                AVCodecContext * const &codecCtx;

                Codec();
                virtual ~Codec();

                virtual void init()=0;
                virtual void init(json::JsonObject cfg)=0;
                virtual void close()=0;

                static enum AVCodecID getCodecIdByName(String codecName);
                static bool supportsPixFmt(String codecName, enum AVPixelFormat pixFmt);
                static enum AVPixelFormat getPixFmtByName(String pixFmtName);
                static String getPixFmtName(enum AVPixelFormat pixFmt);
        };
    }

#endif