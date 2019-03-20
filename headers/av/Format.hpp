#if !defined  _AV_FORMAT_HPP
    #define _AV_FORMAT_HPP

    #include "av/libav.hpp"

    namespace av {

        class Format {

            protected:
                AVFormatContext *_formatCtx;

            public:

                AVFormatContext * const &formatCtx;

                Format();
                virtual ~Format();

                virtual void close();
                virtual void dump() const =0;
        };
    }

#endif