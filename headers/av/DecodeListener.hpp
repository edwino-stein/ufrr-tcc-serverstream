#if !defined  _AV_DECODE_LISTENER_HPP
    #define _AV_DECODE_LISTENER_HPP

    #include "av/libav.hpp"

    namespace av {

        class DecodeListener {

            public:

                virtual ~DecodeListener();
                virtual void onDecoded(AVFrame *frame)=0;
        };
    }

#endif