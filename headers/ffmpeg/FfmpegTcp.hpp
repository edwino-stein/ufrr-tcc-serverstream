#if !defined  _FFMPEG_TCP_HPP
    #define _FFMPEG_TCP_HPP

    #include "types.hpp"
    #include "ffmpeg/FfmpegListener.hpp"
    #include "runtime/LoopInside.hpp"

    namespace ffmpeg {

        class FfmpegTcp : public runtime::LoopInside {

            protected:

                net::IOContext ioCtx;
                net::TCPSocket socket;

                const unsigned int port;
                const size_t bufferSize;
                ffmpeg::FfmpegListener & listener;

                virtual void loop() override;

            public:

                FfmpegTcp(const unsigned int port, const size_t bufferSize, FfmpegListener &listener);
                virtual ~FfmpegTcp();

                virtual void run() override;
                virtual void stop();
        };
    }

#endif
