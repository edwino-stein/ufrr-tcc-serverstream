#if !defined  _LIB_AV_HPP
    #define _LIB_AV_HPP

    #include "types.hpp"

    extern "C" {
        #include <libavcodec/avcodec.h>
        #include <libavdevice/avdevice.h>
        #include <libavfilter/avfilter.h>
        #include <libavformat/avformat.h>
        #include <libavutil/avutil.h>
        #include <libavformat/avio.h>
        #include <libavfilter/buffersink.h>
        #include <libavfilter/buffersrc.h>
    }

    namespace av {

        //Functions
        void initAll();

        //Classes

        //Interfaces
    }

#endif