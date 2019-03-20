#if !defined  _AV_ENCODE_LISTENER_HPP
    #define _AV_ENCODE_LISTENER_HPP

    #include "av/libav.hpp"

    namespace av {

        class EncodeListener {

            public:

                virtual ~EncodeListener();
                virtual void onEncoded(AVPacket *packet)=0;
        };
    }

#endif