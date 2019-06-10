#if !defined  _FFMPEG_LISTENER_HPP
    #define _FFMPEG_LISTENER_HPP

    #include "types.hpp"

    namespace ffmpeg {
        class FfmpegListener {
            public:
                virtual ~FfmpegListener();
                virtual void onFfmpegReceive(unsigned char data[], const size_t length)=0;
        };
    }

#endif
