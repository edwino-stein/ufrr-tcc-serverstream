#if !defined  _AV_VIDEO_OUTPUT_HPP
    #define _AV_VIDEO_OUTPUT_HPP

    #include "av/CodecVideo.hpp"
    #include "av/FormatOutput.hpp"

    namespace av {

        class VideoOutput : public av::CodecVideo, public av::FormatOutput {

            protected:
                virtual void openFile(String &fileName, AVDictionary **options = NULL) override;
                virtual void initFormat(String demuxer, String codec) override;
                virtual void initVideoStream() override;
                virtual void initCodec() override;
                virtual void initCodec(AFunction<void(AVCodecContext *, AVCodec *)> setupCodec) override;

                virtual void closeFile() override;
                virtual void closeFormat() override;
                virtual void closeCodec() override;

            public:

                VideoOutput();
                virtual ~VideoOutput();

                virtual void rescaleTs(AVPacket *packet) const override;
                virtual void encode(AVFrame *frame, EncodeListener *listener) const;

        };
    }

#endif