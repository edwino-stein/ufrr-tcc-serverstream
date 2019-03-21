#if !defined  _AV_FORMAT_HPP
    #define _AV_FORMAT_HPP

    #include "av/libav.hpp"

    namespace av {

        class Format {

            protected:
                AVFormatContext *_formatCtx;

                virtual void openFile(String &fileName)=0;
                virtual void initFormat(String demuxer, String codec)=0;
                virtual void closeFile()=0;
                virtual void closeFormat()=0;

                void dump(String label, bool isOutput) const;

            public:

                AVFormatContext * const &formatCtx;

                Format();
                virtual ~Format();

                virtual void dump() const =0;
        };
    }

#endif